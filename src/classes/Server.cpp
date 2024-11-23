#include "webserv.hpp"

Server::~Server(void) {}
Server::Server(const Server& src) {
	this->deepCopy(src);
}
Server::Server(const ServerConfig& serverConfig) {
	this->_serverConfig = serverConfig;
	this->_socketFd = -1;
}
Server&			Server::operator=(const Server& src) {
	if (this != & src)
		this->deepCopy(src);
	return (*this);
}
void			Server::setServerConfig(const ServerConfig& serverConfig) {
	this->_serverConfig = serverConfig;
}
void			Server::setSocketFd(const int& fd) {
	this->_socketFd = fd;
}
ServerConfig&	Server::getServerConfig(void) {
	return (this->_serverConfig);
}
int				Server::getSocketFd(void) const {
	return (this->_socketFd);
}
sockaddr_in		Server::getAddr(void) const {
	return (this->_addr);
}
bool			Server::listenerSetup(void) {
	this->closeSocketFd();
	this->_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socketFd == -1) {
		Log::error("\t\t\tCould not create socket!");
		return (false);
	}
	int opt = 1;
	if (setsockopt(this->_socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		Log::error("\t\t\tsetsockopt(SO_REUSEADDR) failed!");
		return false;
	}
	this->setAddr();
	if (bind(this->_socketFd, (sockaddr*)&this->_addr, sizeof(this->_addr)) < 0) {
		Log::error("\t\t\tCould not bind socket to the port " + stp_itoa(this->_serverConfig.getPort()) + "!");
		return (false);
	}
	if (listen(this->_socketFd, 1000) == -1) {
		Log::error("\t\t\tCould not listen to " + this->_serverConfig.getListen().toString() + "!");
		return (false);
	}
	Log::log("\t\t\tListening to " + this->_serverConfig.getListen().toString());
	return (true);
}
void			Server::closeSocketFd(void) {
	if (this->_socketFd > 0)
		::close(this->_socketFd);
	this->_socketFd = -1;
}
void			Server::closeSocketFd(const int& socketFd) {
	if (socketFd > 0)
		::close(socketFd);
	this->_requests.erase(socketFd);
}
void			Server::clientSocketCall(void) {
	int clientSocket = ::accept(this->_socketFd, NULL, NULL);
	Log::log("\tServer " + this->_serverConfig.getListen().toString() + " accepted a client");
	char	requestReadingBuffer[this->_serverConfig.getClientMaxBodySize() + 1];
	size_t bytesRead = recv(clientSocket, requestReadingBuffer, sizeof(requestReadingBuffer), 0);
	if (bytesRead > this->_serverConfig.getClientMaxBodySize()) {
		Log::error("Client sent a request bigger than permitted!");
		::close(clientSocket);
		return ;
	}
	requestReadingBuffer[bytesRead] = '\0';
	HttpRequest	clientRequest(requestReadingBuffer);
	Log::info(clientRequest.toString());
	this->serveRequest(clientRequest, clientSocket);
}
void			Server::deepCopy(const Server& src) {
	this->_serverConfig = src._serverConfig;
	this->_socketFd = src._socketFd;
	this->_addr= src._addr;
	this->_requests = src._requests;
}
void			Server::setAddr(void) {
	stp_memset((char*)&this->_addr, 0, sizeof(this->_addr));
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = this->_serverConfig.getHost();
	this->_addr.sin_port = htons(this->_serverConfig.getPort());
}
bool			Server::serveRequest(const HttpRequest& request, const int& clientSocketFd) {
	RequestProcessor	processor(request, this);
	processor.process(clientSocketFd);
/*
	Location	targetRoute = this->_serverConfig.getLocation(request.getTargetRoute());
	if (targetRoute.getPage() == "") {
		HttpResponse	response;
		response.setCode(404);
		Log::error(httpStatusCodeString(response.getCode()));
		response.setType(textHtml);
		response.setContent("<h1>404 Page Not Found</h1>");
		// allResponses.push_back(response);
		// return (allResponses);
	}
	(void)request;
	{
		HttpResponse	response;
		std::string responseBody = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\" /><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" /><title>Calculator</title></head><body>Test Test</body></html>";
		response.setCode(200);
		response.setType(textHtml);
		response.setContent(responseBody);	
		// allResponses.push_back(response);
	}
	
	
	for (size_t i = 0; i < response.size(); ++i) {
		Log::debug("Doing Sht");
		std::string	fullResponse(response[i].toString());
		::send(clientSocketFd, fullResponse.c_str(), fullResponse.size(), 0);
		Log::debug("Done");
	}
	::close(clientSocketFd);
*/
	return (true);
}