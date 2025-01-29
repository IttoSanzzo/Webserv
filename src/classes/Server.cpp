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
short			Server::clientSocketCall(const short& clientSocket) {
	char requestReadingBuffer[this->_serverConfig.getClientMaxBodySize() + 1];
	int bytesRead = recv(clientSocket, requestReadingBuffer, sizeof(requestReadingBuffer), 0);
	if (bytesRead > (int)this->_serverConfig.getClientMaxBodySize()) {
		Log::error("Client sent a request bigger than permitted!");
		::close(clientSocket);
		return (-2);
	}
	else if (bytesRead  == -1)
		return (-3);
	else if (bytesRead == 0)
		return (0);
	requestReadingBuffer[bytesRead] = '\0';
	std::string	requestBufferString(requestReadingBuffer);
	HttpRequest	clientRequest(requestBufferString);
	if (clientRequest.getBody().size() > this->_serverConfig.getClientMaxBodySize()) {
		Log::error("Client sent a request bigger than permitted!");
		::close(clientSocket);
		return (-2);
	}
	Log::info(clientRequest.toString());
	if (this->serveRequest(clientRequest, clientSocket))
		return (clientSocket);
	return (-1);
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
	RequestProcessor	processor(request, this, clientSocketFd);
	return (processor.process());
}