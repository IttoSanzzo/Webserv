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
	while (Cluster::_signalValue == 0) {
		Log::log("Back to Poll.");
		int pollRet = poll(this->_pollFds.data(), this->_pollFds.size(), -1);
		Log::info("Poll return state: " + stp_itoa(pollRet));
		if (pollRet <= 0)
			continue;
		Log::debug("There's data to be read!");
		for (size_t i = 0; i < this->_pollFds.size(); ++i)
			if (this->_pollFds[i].revents & POLLIN) {
				int keepAlive = this->_servers[i].clientSocketCall(); 
				if (keepAlive == 0)
					continue;
				pollfd pfd = {keepAlive, POLL_IN, 0};
				this->_pollFds.push_back(pfd);
			}
	}
}
void				Cluster::deepCopy(const Cluster& src) {
	this->_error = src._error;
	this->_serverConfigs = src._serverConfigs;
	this->_servers = src._servers;
	this->_requests = src._requests;
	this->_pollFds = src._pollFds;
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