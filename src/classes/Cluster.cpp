#include "webserv.hpp"

int Cluster::_signalValue = 0;

Cluster::~Cluster(void) {
	Log::log("Closing cluster...");
	for (size_t i = 0; i < this->_servers.size(); ++i) {
		Log::debug("\tClosing listener for " + this->_servers[i].getServerConfig().getListen().toString());
		this->_servers[i].closeSocketFd();
	}
	this->freePollFd();
}
Cluster::Cluster(const Cluster& src) {
	this->deepCopy(src);
}
Cluster::Cluster(std::string& configurationFilePath) {
	Log::log("Parsing configuration file");
	this->_pollFd = NULL;
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
pollfd*				Cluster::getPollFd(void) const {
	return (this->_pollFd);
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
		int pollRet = poll(this->_pollFd, this->_servers.size(), -1);
		Log::info("Poll return state: " + stp_itoa(pollRet));
		if (pollRet <= 0)
			continue;
		Log::debug("There's data to be read!");
		for (size_t i = 0; i < this->_servers.size(); ++i) {
			if (this->_pollFd[i].revents & POLLIN) {
				Log::debug("Server Reading : " + stp_itoa(i));
				this->_servers[i].clientSocketCall();
			}
		}
	}
}
void				Cluster::deepCopy(const Cluster& src) {
	this->_error = src._error;
	this->_serverConfigs = src._serverConfigs;
	this->_servers = src._servers;
	this->_requests = src._requests;
	if (src._pollFd != NULL) {
		size_t pollFdSize = this->_serverConfigs.getSize();
		this->_pollFd = new pollfd[pollFdSize];
		for (size_t i = 0; i < pollFdSize; ++i)
			this->_pollFd[i] = src._pollFd[i];
	}
	else
		this->_pollFd = NULL;
}
void				Cluster::signalHandler(int signal) {
	Log::log("SIGINT signal Detected");
	Cluster::_signalValue = signal;
}
void				Cluster::setPollFd(pollfd* pollFd) {
	if (pollFd != NULL) {
		this->freePollFd();
		this->_pollFd = pollFd;
	}
}
void				Cluster::freePollFd(void) {
	if (this->_pollFd != NULL) {
		delete[] this->_pollFd;
		this->_pollFd = NULL;
	}

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
	this->setPollFd(new	pollfd[this->_servers.size()]);
	for (size_t i = 0; i < this->_servers.size(); ++i) {
		this->_pollFd[i].fd = this->_servers[i].getSocketFd();
		this->_pollFd[i].events = POLL_IN;
		this->_pollFd[i].revents = 0;
	}
	Log::log("\tPollFd Setted up.");
}