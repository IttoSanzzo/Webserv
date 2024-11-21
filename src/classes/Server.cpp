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
	this->_addr.sin_addr.s_addr = htonl(this->_serverConfig.getHost());
	this->_addr.sin_port = htons(this->_serverConfig.getPort());
}
void	Server::closeFd(void) {
	if (this->_socketFd > 0)
		::close(this->_socketFd);
	this->_socketFd = -1;
}
void	Server::closeSocket(const int& socket) {
	if (socket > 0)
		::close(socket);
	this->_requests.erase(socket);
}