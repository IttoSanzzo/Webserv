#include "webserv.hpp"

int Cluster::_signalValue = 0;

/* C. Constructors */
Cluster::~Cluster(void) {
	Log::log("Closing cluster...");
	for (size_t i = 0; i < this->_servers.size(); ++i) {
		Log::debug("\tClosing listener for " + this->_servers[i].getServerConfig().getListen().toString());
		this->_servers[i].closeSocketFd();
	}
}
Cluster::Cluster(const Cluster& src) {
	this->deepCopy(src);
}
Cluster&	Cluster::operator=(const Cluster& src) {
	if (this != &src)
		this->deepCopy(src);
	return (*this);
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

/* G. Getters */
int					Cluster::getError(void) const {
	return (this->_error);
}
ServerConfigArray&	Cluster::getServerConfigArray(void) {
	return (this->_serverConfigs);
}
ServerConfig&		Cluster::getServerConfig(const size_t& pos) {
	return (this->_serverConfigs.GetServer(pos));
}
size_t				Cluster::getServerConfigSize(void) const {
	return (this->_serverConfigs.GetSize());
}

/* PUB0. Core */
void	Cluster::runCluster(void) {
	::signal(SIGINT, signalHandler);
	Log::log("Starting cluster...");
	for (size_t i = 0; i < this->_serverConfigs.GetSize(); ++i)
		this->_servers.push_back(Server(_serverConfigs.GetServer(i)));
	Log::log("\tOpening listeners...");
	for (size_t i = 0; i < this->_servers.size(); ++i)
		if (this->_servers[i].listenerSetup() == false)
			return ;
	Log::log("\tSetting Poll...");
	pollfd	socketPoll[this->_servers.size()];
	for (size_t i = 0; i < this->_servers.size(); ++i) {
		socketPoll[i].fd = this->_servers[i].getSocketFd();
		socketPoll[i].events = POLL_IN;
		socketPoll[i].revents = 0;
	}
	while (Cluster::_signalValue == 0) {
		Log::debug("RunPoll");
		int ret = poll(socketPoll, this->_servers.size(), -1);
		Log::debug("State: " + stp_itoa(ret));
		if (ret > 0) {
			Log::debug("Dados para leitura!");
			for (size_t i = 0; i < this->_servers.size(); ++i) {
				if (socketPoll[i].revents & POLLIN) {
					Log::debug("Server Reading : " + stp_itoa(i));
					int testSocket = ::accept(socketPoll[i].fd, NULL, NULL);

					Log::debug(stp_itoa(testSocket));
					char	bufff[3000];
					int bytes = recv(testSocket, bufff, sizeof(bufff), 0);
					Log::debug("Bytes: " + stp_itoa(bytes));
					bufff[bytes] = '\0';
					Log::debug("Retur: " + std::string(bufff));
					
					std::string responseBody = "<!DOCTYPE html>\n<html lang=\"en\">\n\t<head>\n\t\t<meta charset=\"UTF-8\" />\n\t\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n\t\t<title>Calculator</title>\n\t</head>\n\t<body>\n\t\tTest Test\n\t</body>\n</html>";
					HttpResponse	response;
					response.setCode(200);
					response.setType(textHtml);
					response.setContent(responseBody);

					Log::debug("Doing Sht");
					std::string	fullResponse = response.toString();
					::send(testSocket, fullResponse.c_str(), fullResponse.size(), 0);
					Log::debug("Done");
					::close(testSocket);
				}
			}
		}
	}
}

/* PRI0. Private Functions */
void	Cluster::deepCopy(const Cluster& src) {
	this->_error = src._error;
	this->_serverConfigs = src._serverConfigs;
	this->_servers = src._servers;
	this->_requests = src._requests;
}
void	Cluster::signalHandler(int signal) {
	Log::log("SIGINT signal Detected");
	Cluster::_signalValue = signal;
}