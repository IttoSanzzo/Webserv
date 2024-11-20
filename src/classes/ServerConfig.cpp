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
	this->deepCopy(src);
}
ServerConfig&	ServerConfig::operator=(const ServerConfig& src) {
	if (this != &src)
		this->deepCopy(src);
	return (*this);
}
ServerConfig::ServerConfig(const JsonNode& configJson) {
	this->setPort(configJson);
	this->setHost(configJson);
	this->setServerName(configJson);
	this->setRoot(configJson);
	this->setIndex(configJson);
	this->setClientMaxBodySize(configJson);
	this->setAutoindex(configJson);
	this->setErrorPages(configJson);
	this->setLocations(configJson);
	this->_listen_fd = 0;
}

/* S. Setters */
void	ServerConfig::setPort(const std::string& port) {
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
void	ServerConfig::setServerName(const std::string& server_name) {
	this->_server_name = server_name;
}
void	ServerConfig::setRoot(const std::string& root) {
	// TODO ServerConfig::setRoot
	this->_root = root;
}
void	ServerConfig::setIndex(const std::string& index) {
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
u_int16_t	ServerConfig::getPort(void) const {
	return (this->_port);
}
in_addr_t	ServerConfig::getHost(void) const {
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
bool		ServerConfig::getAutoindex(void) const {
	return (this->_autoindex);
}
size_t		ServerConfig::getClientMaxBodySize(void) const {
	return (this->_client_max_body_size);
}
std::string	ServerConfig::getErrorPage(const short& pos) {
	return (this->_error_pages[pos]);
}
Location	ServerConfig::getLocation(const std::string& page) {
	Location	returner = this->_locations[page];
	if (returner.getPage() == page && returner.getReturner() != "")
		return (this->getLocation(returner.getReturner()));
	return (returner);
}
int			ServerConfig::getFd(void) const {
	return (this->_listen_fd);
}

/* PUB0. Core */
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

/* PRI0. JsonParsing */
void		ServerConfig::setPort(const JsonNode& configJson) {
	try {
		this->_port = configJson.TryGetInt("port");
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (ServerConfig::ErrorException("\"port\" Element should be integer!"));
		throw (ServerConfig::ErrorException(ex.what()));
	}
}
void		ServerConfig::setHost(const JsonNode& configJson) {
	try {
		this->setHost(configJson.TryGetString("host"));
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (ServerConfig::ErrorException("\"host\" Element should be string!"));
		throw (ServerConfig::ErrorException(ex.what()));
	}
}
void		ServerConfig::setServerName(const JsonNode& configJson) {
	try {
		this->setServerName(configJson.TryGetString("server_name"));
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (ServerConfig::ErrorException("\"server_name\" Element should be string!"));
		throw (ServerConfig::ErrorException(ex.what()));
	}
}
void		ServerConfig::setRoot(const JsonNode& configJson) {
	try {
		this->setRoot(configJson.TryGetString("root"));
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (ServerConfig::ErrorException("\"root\" Element should be string!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			this->_root = "docs/fusion_web/";
		else
			throw (ServerConfig::ErrorException(ex.what()));
	}
}
void		ServerConfig::setIndex(const JsonNode& configJson) {
	try {
		this->setIndex(configJson.TryGetString("index"));
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (ServerConfig::ErrorException("\"index\" Element should be string!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			this->_index = "index.html";
		else
			throw (ServerConfig::ErrorException(ex.what()));
	}
}
void		ServerConfig::setClientMaxBodySize(const JsonNode& configJson) {
	try {
		this->setClientMaxBodySize(configJson.TryGetInt("client_max_size_body"));
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (ServerConfig::ErrorException("\"client_max_size_body\" Element should be integer!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			this->_client_max_body_size = MAX_CLI_SIZE;
		else
			throw (ServerConfig::ErrorException(ex.what()));
	}
}
void		ServerConfig::setAutoindex(const JsonNode& configJson) {
	try {
		this->setAutoindex(configJson.TryGetBool("autoindex"));
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (ServerConfig::ErrorException("\"autoindex\" Element should be boolean!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			this->_autoindex = false;
		else
			throw (ServerConfig::ErrorException(ex.what()));
	}
}
void		ServerConfig::setErrorPages(const JsonNode& configJson) {
	JsonChildren errorChildren;
	try {
		errorChildren = configJson.TryGetChildren("error_pages");
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (ServerConfig::ErrorException("\"error_pages\" Element should be children!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			return ;
		else
			throw (ServerConfig::ErrorException(ex.what()));
	}
	for (size_t i = 0; i < errorChildren.GetSize(); ++i)
		this->parseErrorChild(errorChildren.GetChildNode(i));
}
void		ServerConfig::setLocations(const JsonNode& configJson) {
	JsonChildren locationsChildren;
	try {
		locationsChildren = configJson.TryGetChildren("locations");
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (ServerConfig::ErrorException("\"locations\" Element should be children!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			return ;
		else
			throw (ServerConfig::ErrorException(ex.what()));
	}
	for (size_t i = 0; i < locationsChildren.GetSize(); ++i) {
		Location location = Location(locationsChildren.GetChildNode(i));
		this->_locations[location.getPage()] = location;
	}
}
void		ServerConfig::parseErrorChild(const JsonNode& errorChild) {
	std::string	page = this->parseErrorElement(errorChild, "page");
	if (stp_checkSufix(page, ".html") == false)
		throw (ServerConfig::ErrorException("\"error_pages.page\" Element should be an html file!"));
	std::string	codes = this->parseErrorElement(errorChild, "codes");
	if (stp_stringIsNumericOrSpaced(codes) == false)
		throw (ServerConfig::ErrorException("\"error_pages.codes\" Element should have only numbers!"));
	std::string* allCodes = stp_split(codes, ' ');
	size_t	allCodesSize = stp_stringArrayLength(allCodes);
	for (size_t i = 0; i < allCodesSize; ++i)
		this->_error_pages[std::atoi(allCodes[i].c_str())] = page;
	delete[] allCodes;
}
std::string	ServerConfig::parseErrorElement(const JsonNode& errorChild, const std::string& element) {
	try {
		return (errorChild.TryGetString(element));
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (ServerConfig::ErrorException("\"error_pages." + element + "\" Element should be string!"));
		throw (ServerConfig::ErrorException(ex.what()));
	}
}
/* PRIU. Utils */
void	ServerConfig::deepCopy(const ServerConfig& src) {
	this->_port = src._port;
	this->_host = src._host;
	this->_server_name = src._server_name;
	this->_root = src._root;
	this->_index = src._index;
	this->_autoindex = src._autoindex;
	this->_listen_fd = src._listen_fd;
	this->_client_max_body_size = src._client_max_body_size;
	this->_error_pages = src._error_pages;
	this->_locations = src._locations;
	this->_server_address = src._server_address;
}