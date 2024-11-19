#include "ServerConfig.hpp"

/* C. Constructors */
ServerConfig::~ServerConfig(void) {}
ServerConfig::ServerConfig(void) {
	this->_port = 0;
	this->_host = 0;
	this->_server_name = "";
	this->_root = "";
	this->_index = "";
	this->_autoindex = false;
	this->_listen_fd = 0;
	this->_client_max_body_size = MAX_CLI_SIZE;
	this->initErrorPages();
}
ServerConfig::ServerConfig(const ServerConfig& src) {
	if (this != &src){
		this->_port = src._port;
		this->_host = src._host;
		this->_server_name = src._server_name;
		this->_root = src._root;
		this->_index = src._index;
		this->_autoindex = src._autoindex;
		this->_listen_fd = src._listen_fd;
		this->_client_max_body_size = src._client_max_body_size;
		this->_error_pages = src._error_pages;
		this->_server_address = src._server_address;
	}
	return ;
}
ServerConfig&	ServerConfig::operator=(const ServerConfig& src) {
	if (this != &src){
		this->_port = src._port;
		this->_host = src._host;
		this->_server_name = src._server_name;
		this->_root = src._root;
		this->_index = src._index;
		this->_autoindex = src._autoindex;
		this->_listen_fd = src._listen_fd;
		this->_client_max_body_size = src._client_max_body_size;
		this->_error_pages = src._error_pages;
		this->_server_address = src._server_address;
	}
	return (*this);
}

/* S. Setters */
void	ServerConfig::setPort(std::string port) {
	for (size_t i = 0; i < port.length(); i++)
		if (!std::isdigit(port[i]))
			throw ErrorException(ERR_PORT);
	unsigned int	new_port = std::atoi(port.c_str());
	if (new_port < 1 || new_port > MAX_PORT_NUM)
		throw ErrorException(ERR_PORT);
	this->_port = (u_int16_t)new_port;
}
void	ServerConfig::setHost(std::string host) {
	if (host == "localhost")
		host = "127.0.0.1";
	else if (!isValidHost(host))
		throw ErrorException(ERR_HOST);
	this->_host = ws_inet_addr(host.data());
}
void	ServerConfig::setServerName(std::string server_name) {
	this->_server_name = server_name;
}
void	ServerConfig::setRoot(std::string root) {
	// TODO ServerConfig::setRoot
	this->_root = root;
}
void	ServerConfig::setIndex(std::string index) {
	this->_index = index;
}
void	ServerConfig::setAutoindex(bool autoindex) {
	this->_autoindex = autoindex;
}
void	ServerConfig::setFd(int listen_fd) {
	this->_listen_fd = listen_fd;
}
void	ServerConfig::setClientMaxBodySize(int body_size) {
	this->_client_max_body_size = body_size;
}

/* G. Getters */
u_int16_t		ServerConfig::getPort(void) const {
	return (this->_port);
}
in_addr_t		ServerConfig::getHost(void) const {
	return (this->_host);
}
std::string	ServerConfig::getServerName(void) const {
	return (this->_server_name);
}
std::string	ServerConfig::getRoot(void) const {
	return (this->_root);
}
std::string	ServerConfig::getIndex(void) const {
	return (this->_index);
}
bool			ServerConfig::getAutoindex(void) const {
	return (this->_autoindex);
}
int			ServerConfig::getFd(void) const {
	return (this->_listen_fd);
}
size_t		ServerConfig::getClientMaxBodySize(void) const {
	return (this->_client_max_body_size);
}

/* 0. Core */
void	ServerConfig::initErrorPages(void) {
	this->_error_pages[301] = "";
	this->_error_pages[302] = "";
	this->_error_pages[400] = "";
	this->_error_pages[401] = "";
	this->_error_pages[402] = "";
	this->_error_pages[403] = "";
	this->_error_pages[404] = "";
	this->_error_pages[405] = "";
	this->_error_pages[406] = "";
	this->_error_pages[500] = "";
	this->_error_pages[501] = "";
	this->_error_pages[502] = "";
	this->_error_pages[503] = "";
	this->_error_pages[504] = "";
	this->_error_pages[505] = "";
}
bool	ServerConfig::isValidHost(std::string host) const {
	struct sockaddr_in	sockaddr;
  	return (ws_inet_pton(AF_INET, host.c_str(), &(sockaddr.sin_addr)) ? true : false);
}