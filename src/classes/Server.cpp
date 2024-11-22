#include "webserv.hpp"

/* C. Constructors */
Server::~Server(void) {}
Server::Server(const Server& src) {
	this->deepCopy(src);
}
Server&	Server::operator=(const Server& src) {
	if (this != & src)
		this->deepCopy(src);
	return (*this);
}
Server::Server(const ServerConfig& serverConfig) {
	this->_serverConfig = serverConfig;
	this->_socketFd = -1;
}

/* S. Setters */
void	Server::setServerConfig(const ServerConfig& serverConfig) {
	this->_serverConfig = serverConfig;
}
void	Server::setSocketFd(const int& fd) {
	this->_socketFd = fd;
}

/* G. Getters */
ServerConfig&	Server::getServerConfig(void) {
	return (this->_serverConfig);
}
int				Server::getSocketFd(void) const {
	return (this->_socketFd);
}
sockaddr_in		Server::getAddr(void) const {
	return (this->_addr);
}

/* PUB0. Core */
bool	Server::listenerSetup(void) {
	this->closeSocketFd();
	this->_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socketFd == -1) {
		Log::error("\t\tCould not create socket!");
		return (false);
	}
	int opt = 1;
	if (setsockopt(this->_socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		Log::error("\t\tsetsockopt(SO_REUSEADDR) failed!");
		return false;
	}
	this->setAddr();
	if (bind(this->_socketFd, (sockaddr*)&this->_addr, sizeof(this->_addr)) < 0) {
		Log::error("\t\tCould not bind socket to the port " + stp_itoa(this->_serverConfig.getPort()) + "!");
		return (false);
	}
	if (listen(this->_socketFd, 1000) == -1) {
		Log::error("\t\tCould not listen to " + this->_serverConfig.getListen().toString() + "!");
		return (false);
	}
	Log::log("\t\tListening to " + this->_serverConfig.getListen().toString());
	return (true);
}
void	Server::closeSocketFd(void) {
	if (this->_socketFd > 0)
		::close(this->_socketFd);
	this->_socketFd = -1;
}
void	Server::closeSocketFd(const int& socketFd) {
	if (socketFd > 0)
		::close(socketFd);
	this->_requests.erase(socketFd);
}

/* PRI0. Private Functions */
void	Server::deepCopy(const Server& src) {
	this->_serverConfig = src._serverConfig;
	this->_socketFd = src._socketFd;
	this->_addr= src._addr;
	this->_requests = src._requests;
}
void	Server::setAddr(void) {
	stp_memset((char*)&this->_addr, 0, sizeof(this->_addr));
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = this->_serverConfig.getHost();
	this->_addr.sin_port = htons(this->_serverConfig.getPort());
}