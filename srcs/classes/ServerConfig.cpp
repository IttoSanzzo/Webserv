/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcosv2 <marcosv2@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 22:43:39 by marcosv2          #+#    #+#             */
/*   Updated: 2024/05/28 05:40:20 by marcosv2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

/* Commmons */

ServerConfig::~ServerConfig(void) {}

ServerConfig::ServerConfig(void) {
	this->_port = 0;
	this->_host = 0;
	this->_server_name = "";
	this->_root = "";
	this->_index = "";
	this->_autoindex = false;
	this->_listen_fd = 0;
	this->_cli_max_body_size = MAX_CLI_SIZE;
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
		this->_cli_max_body_size = src._cli_max_body_size;
		this->_error_pages = src._error_pages;
		// TODO location uncomment
		// this->_locations = src._locations;
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
		this->_cli_max_body_size = src._cli_max_body_size;
		this->_error_pages = src._error_pages;
		// TODO location uncomment
		// this->_locations = src._locations;
		this->_server_address = src._server_address;
	}
	return (*this);
}

/* Setters */

void	ServerConfig::setPort(std::string port) {
	checkToken(port);
	for (size_t i = 0; i < port.length(); i++)
		if (!std::isdigit(port[i]))
			throw ErrorException(ERR_PORT);
	unsigned int	new_port = std::atoi(port.c_str());
	if (new_port < 1 || new_port > MAX_PORT_NUM)
		throw ErrorException(ERR_PORT);
	this->_port = (u_int16_t)new_port;
}

void	ServerConfig::setHost(std::string host) {
	checkToken(host);
	if (host == "localhost")
		host = "127.0.0.1";
	if (!isValidHost(host))
		throw ErrorException(ERR_HOST);
	this->_host = ws_inet_addr(host.data());
}

void	ServerConfig::setServerName(std::string server_name) {
	checkToken(server_name);
	this->_server_name = server_name;
}

void	ServerConfig::setRoot(std::string root) {
	// TODO ServerConfig::setRoot
	checkToken(root);
	this->_root = root;
}

void	ServerConfig::setIndex(std::string index) {
	checkToken(index);
	this->_index = index;
}

void	ServerConfig::setAutoindex(std::string autoindex) {
	checkToken(autoindex);
	if (autoindex != "on" && autoindex != "off")
		throw ErrorException(ERR_AUTOINDEX);
	autoindex == "on" ? this->_autoindex = true : this->_autoindex = false;
}

void	ServerConfig::setFd(int listen_fd) {
	this->_listen_fd = listen_fd;
}

void	ServerConfig::setClientMaxBodySize(std::string body_size) {
	checkToken(body_size);
	for (size_t i = 0; i < body_size.length(); i++)
		if (body_size[i] < '0' || body_size[i] > '9')
			throw ErrorException(ERR_MAXBODY);
	unsigned long	new_body_size = std::atoi(body_size.c_str());
	if (!new_body_size)
		throw ErrorException(ERR_MAXBODY);
	this->_cli_max_body_size = new_body_size;
}

/*
void	ServerConfig::setErrorPages(std::vector<std::string> &parameter) {
	this->_error_pages = ;
}
*/

/*
void	ServerConfig::setLocation(std::string nameLocation, std::vector<std::string> parameter) {
	this->_ = ;
}
*/

/* Getters */

const u_int16_t&	ServerConfig::getPort(void) const {
	return (this->_port);
}

const in_addr_t&	ServerConfig::getHost(void) const {
	return (this->_host);
}

const std::string&	ServerConfig::getServerName(void) const {
	return (this->_server_name);
}

const std::string&	ServerConfig::getRoot(void) const {
	return (this->_root);
}

const std::string&	ServerConfig::getIndex(void) const {
	return (this->_index);
}

const bool&			ServerConfig::getAutoindex(void) const {
	return (this->_autoindex);
}

const int&			ServerConfig::getFd(void) const {
	return (this->_listen_fd);
}

const size_t&		ServerConfig::getClientMaxBodySize(void) const {
	return (this->_cli_max_body_size);
}

/* Others */

void	ServerConfig::initErrorPages(void) {
	_error_pages[301] = "";
	_error_pages[302] = "";
	_error_pages[400] = "";
	_error_pages[401] = "";
	_error_pages[402] = "";
	_error_pages[403] = "";
	_error_pages[404] = "";
	_error_pages[405] = "";
	_error_pages[406] = "";
	_error_pages[500] = "";
	_error_pages[501] = "";
	_error_pages[502] = "";
	_error_pages[503] = "";
	_error_pages[504] = "";
	_error_pages[505] = "";
}

bool	ServerConfig::isValidHost(std::string host) const {
	struct sockaddr_in	sockaddr;
  	return (ws_inet_pton(AF_INET, host.c_str(), &(sockaddr.sin_addr)) ? true : false);
}

void	ServerConfig::checkToken(std::string &parameter) {
	size_t	pos = parameter.rfind(';');
	if (parameter == "" || pos != parameter.size() - 1)
		throw ErrorException("Token is invalid");
	parameter.erase(pos);
}
