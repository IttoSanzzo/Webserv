#include "webserv.hpp"

int Cluster::_signalValue = 0;

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
int					Cluster::getError(void) const {
	return (this->_error);
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
void				Cluster::runCluster(void) {
	::signal(SIGINT, signalHandler);
	Log::log("Starting cluster...");
	for (size_t i = 0; i < this->_serverConfigs.getSize(); ++i)
		this->_servers.push_back(Server(_serverConfigs.getServer(i)));
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
					bufff[bytes] = '\0';
					Log::debug(std::string(bufff));
					HttpRequest	request(bufff);
					Log::info("Method: " + HttpRequest::methodToString(request.getMethod()));
					Log::info("Route: " + request.getTargetRoute());
					Log::info("Protocol: " + request.getProtocol());
					Log::info("Host: " + request.getHost());
					Log::info("Agent: " + request.getUserAgent());
					Log::info("Accept: " + request.getAccept(0));
					Log::info("Encoding: " + request.getAcceptEncoding(0));
					Log::info("Token: " + request.getOther("Postman-Token"));
					
					std::string responseBody = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\" /><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" /><title>Calculator</title></head><body>Test Test</body></html>";
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
void				Cluster::deepCopy(const Cluster& src) {
	this->_error = src._error;
	this->_serverConfigs = src._serverConfigs;
	this->_servers = src._servers;
	this->_requests = src._requests;
}
void				Cluster::signalHandler(int signal) {
	Log::log("SIGINT signal Detected");
	Cluster::_signalValue = signal;
}