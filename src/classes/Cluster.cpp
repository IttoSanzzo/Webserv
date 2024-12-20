#include "webserv.hpp"

int Cluster::_signalValue = 0;

Cluster::~Cluster(void) {
	Log::log("Closing cluster...");
	for (size_t i = 0; i < this->_pollFds.size(); ++i) {
		// Log::debug("\tClosing listener for " + this->_servers[i].getServerConfig().getListen().toString());
		::close(this->_pollFds[i].fd);
	}
}
Cluster::Cluster(const Cluster& src) {
	this->deepCopy(src);
}
Cluster::Cluster(std::string& configurationFilePath) {
	Log::log("Parsing configuration file");
	this->_serverConfigs = ServerConfigArray(configurationFilePath);
	this->_error = this->_serverConfigs.getError();
	if (this->_error != 0)
		return ;
	this->_requests = 0;
	Log::log("Configuration File parsed");
}
Cluster&			Cluster::operator=(const Cluster& src) {
	if (this != &src)
		this->deepCopy(src);
	return (*this);
}
ServerConfigArray&	Cluster::getServerConfigArray(void) {
	return (this->_serverConfigs);
}
ServerConfig&		Cluster::getServerConfig(const size_t& pos) {
	return (this->_serverConfigs.getServer(pos));
}
size_t				Cluster::getServerConfigSize(void) const {
	return (this->_serverConfigs.getSize());
}
int					Cluster::getError(void) const {
	return (this->_error);
}
std::vector<pollfd>	Cluster::getPollFds(void) const {
	return (this->_pollFds);
}
void				Cluster::runCluster(void) {
	Log::log("Starting cluster...");
	::signal(SIGINT, signalHandler);
	if (this->serversStart() == false)
			return ;
	Log::log("\tServers setted up.");
	this->pollFdSetup();
	this->runPoll();
}
void				Cluster::runPoll(void) {
	Log::log("Starting poll!");
	size_t	pollRunCount = 0;
	while (Cluster::_signalValue == 0) {
		Log::log("Back to Poll for count " + stp_itoa(pollRunCount++) + " (" + stp_itoa(this->_pollFds.size() - this->_servers.size()) + " clients)");
		int pollRet = poll(this->_pollFds.data(), this->_pollFds.size(), CLIENTTIMEOUT * 500);
		if (pollRet > 0) {
			Log::debug("There's data to be read!");
			for (size_t i = 0 ; i < this->_pollFds.size(); ++i)
				if (this->_pollFds[i].revents & POLLIN) {
					this->handleClient(i);
					break;
				}
		}
		this->checkTimeouts(time(NULL));
	}
}
void				Cluster::deepCopy(const Cluster& src) {
	this->_error = src._error;
	this->_serverConfigs = src._serverConfigs;
	this->_servers = src._servers;
	this->_requests = src._requests;
	this->_pollFds = src._pollFds;
	this->_clientConnMap = src._clientConnMap;
}
void				Cluster::signalHandler(int signal) {
	Log::log("SIGINT signal Detected");
	Cluster::_signalValue = signal;
}
void				Cluster::setPollFds(std::vector<pollfd> pollFds) {
	this->_pollFds = pollFds;
}
bool				Cluster::serversStart(void) {
	Log::log("\tSetting servers up...");
	for (size_t i = 0; i < this->_serverConfigs.getSize(); ++i)
		this->_servers.push_back(Server(_serverConfigs.getServer(i)));
	return (this->serversListenersSetup());
}
bool				Cluster::serversListenersSetup(void) {
	Log::log("\t\tOpening server's listeners...");
	for (size_t i = 0; i < this->_servers.size(); ++i)
		if (this->_servers[i].listenerSetup() == false)
			return (false);
	Log::log("\t\tListeners open.");
	return (true);
}
void				Cluster::pollFdSetup(void) {
	Log::log("\tSetting PollFd...");
	for (size_t i = 0; i < this->_servers.size(); ++i) {
		pollfd pfd = {this->_servers[i].getSocketFd() , POLL_IN, 0};
		this->_pollFds.push_back(pfd);
	}
	Log::log("\tPollFd Setted up.");
}
void				Cluster::handleClient(const size_t& pollIndex) {
	Log::debug("Data Found For _pollFds[" + stp_itoa(pollIndex) + std::string("]"));
	if (pollIndex >= this->_servers.size())
		this->handleClientRequest(pollIndex);
	else
		this->acceptNewClient(pollIndex);
}
void				Cluster::acceptNewClient(const size_t& pollIndex) {
	Log::log("\tServer " + this->_serverConfigs.getServer(pollIndex).getListen().toString() + " accepting a new client");
	int newClient = ::accept(this->_pollFds[pollIndex].fd, NULL, NULL);
	if (newClient == -1) {
		Log::error("There was an error accepting new client.");
		return;
	}
	pollfd pfd = {newClient, POLL_IN, 0};
	this->_pollFds.push_back(pfd);
	t_clientConn	cliConnStruct;
	cliConnStruct.server = &(this->_servers[pollIndex]);
	cliConnStruct.fd = newClient;
	cliConnStruct.last_activity = time(NULL);
	this->_clientConnMap[newClient] = cliConnStruct;
	Log::log("\tNew client in pos " + stp_itoa(this->_pollFds.size() - 1) + " for FD " + stp_itoa(newClient));
}
void				Cluster::handleClientRequest(const size_t& pollIndex) {
	if (this->_clientConnMap[this->_pollFds[pollIndex].fd].server != NULL) {
		Log::log("\tServer " + this->_clientConnMap[this->_pollFds[pollIndex].fd].server->getServerConfig().getListen().toString() + " accepting client request");
		int keepAliveFd = this->_clientConnMap[this->_pollFds[pollIndex].fd].server->clientSocketCall(this->_pollFds[pollIndex].fd);
		this->_clientConnMap[this->_pollFds[pollIndex].fd].last_activity = time(NULL);
		if (keepAliveFd > 0)
			return ;
	}
	this->closePollFd(pollIndex);
}
void				Cluster::checkTimeouts(const time_t& now) {
	for (size_t i = this->_pollFds.size() - 1; i >= this->_servers.size() ; --i)
		if (this->_clientConnMap[this->_pollFds[i].fd].timeout_safe(now) == false)
			this->closePollFd(i);
}
void				Cluster::closePollFd(const size_t& pollIndex) {
	Log::log("\tClosing pos " + stp_itoa(pollIndex) + " FD " + stp_itoa(this->_pollFds[pollIndex].fd));
	::close(this->_pollFds[pollIndex].fd);
	this->_pollFds.erase(this->_pollFds.begin() + pollIndex);
	this->_clientConnMap.erase(this->_pollFds[pollIndex].fd);
}