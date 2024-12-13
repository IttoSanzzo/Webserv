#include "webserv.hpp"

ServerConfig::~ServerConfig(void) {}
ServerConfig::ServerConfig(void) {
	this->_listen.port = 0;
	this->_listen.host = 0;
	this->_server_name = "";
	this->_dataDirectory = "";
	this->_root = "";
	this->_index = "";
	this->_autoindex = false;
	this->_client_max_body_size = MAX_CLI_SIZE;
}
ServerConfig::ServerConfig(const ServerConfig& src) {
	this->deepCopy(src);
}
ServerConfig::ServerConfig(const JsonNode& configJson) {
	this->setPort(configJson);
	this->setHost(configJson);
	this->setServerName(configJson);
	this->setDataDirectory(configJson);
	this->setRoot(configJson);
	this->setIndex(configJson);
	this->setClientMaxBodySize(configJson);
	this->setAutoindex(configJson);
	this->setErrorPages(configJson);
	this->setRoutes(configJson);
}
ServerConfig&	ServerConfig::operator=(const ServerConfig& src) {
	if (this != &src)
		this->deepCopy(src);
	return (*this);
}
void			ServerConfig::setListen(const t_listen& listen) {
	this->_listen = listen;
}
void			ServerConfig::setPort(const std::string& port) {
	for (size_t i = 0; i < port.length(); i++)
		if (!std::isdigit(port[i]))
			throw ErrorException(ERR_PORT);
	unsigned int	new_port = std::atoi(port.c_str());
	if (new_port < 1 || new_port > MAX_PORT_NUM)
		throw ErrorException(ERR_PORT);
	this->_listen.port = (u_int16_t)new_port;
}
void			ServerConfig::setHost(std::string host) {
	if (host == "localhost")
		host = "127.0.0.1";
	else if (!isValidHost(host))
		throw ErrorException(ERR_HOST);
	this->_listen.host = ws_inet_addr(host.data());
}
void			ServerConfig::setServerName(const std::string& server_name) {
	this->_server_name = server_name;
}
void			ServerConfig::setDataDirectory(const std::string& dataDirectory) {
	this->_dataDirectory = dataDirectory;
}
void			ServerConfig::setRoot(const std::string& root) {
	// TODO ServerConfig::setRoot
	this->_root = root;
}
void			ServerConfig::setIndex(const std::string& index) {
	this->_index = index;
}
void			ServerConfig::setAutoindex(bool autoindex) {
	this->_autoindex = autoindex;
}
void			ServerConfig::setClientMaxBodySize(int body_size) {
	this->_client_max_body_size = body_size;
}
t_listen		ServerConfig::getListen(void) const {
	return (this->_listen);
}
u_int16_t		ServerConfig::getPort(void) const {
	return (this->_listen.port);
}
in_addr_t		ServerConfig::getHost(void) const {
	return (this->_listen.host);
}
std::string		ServerConfig::getServerName(void) const {
	return (this->_server_name);
}
std::string		ServerConfig::getDataDirectory(void) const {
	return (this->_dataDirectory);
}
std::string		ServerConfig::getRoot(void) const {
	return (this->_root);
}
std::string		ServerConfig::getIndex(void) const {
	return (this->_index);
}
bool			ServerConfig::getAutoindex(void) const {
	return (this->_autoindex);
}
size_t			ServerConfig::getClientMaxBodySize(void) const {
	return (this->_client_max_body_size);
}
std::string		ServerConfig::getErrorPage(const short& pos) {
	return (this->_error_pages[pos]);
}
Route			ServerConfig::getRoute(const std::string& page) {
	Route	returner = this->_routes[page];
	if (returner.getRoutePath() == page && returner.getReturner() != "")
		return (this->getRoute(returner.getReturner()));
	return (returner);
}
std::map<short, std::string>&	ServerConfig::getErrorPagesMap(void) {
	return (this->_error_pages);
}
std::map<std::string, Route>&	ServerConfig::getRoutesMap(void) {
	return (this->_routes);
}
std::string		ServerConfig::toString(void) {
	std::string	serverConfigInfo = "";
	serverConfigInfo += "\tHost.......: " + ws_inet_ntoa(ntohl(this->_listen.host)) + "\n";
	serverConfigInfo += "\tPort.......: " + stp_itoa(this->_listen.port) + "\n";
	serverConfigInfo += "\tName.......: " + this->_server_name + "\n";
	serverConfigInfo += "\tDataDir....: " + this->_dataDirectory + "\n";
	serverConfigInfo += "\tRoot.......: " + this->_root + "\n";
	serverConfigInfo += "\tIndex......: " + this->_index + "\n";
	serverConfigInfo += "\tAutoindex..: " + stp_btoa(this->_autoindex) + "\n";
	serverConfigInfo += "\tCliMaxSize.: " + stp_itoa(this->_client_max_body_size);
	if (this->_error_pages.size() > 0) {
		serverConfigInfo += std::string("\n\tErrorPages->");
		for (std::map<short, std::string>::iterator i = this->_error_pages.begin(); i != this->_error_pages.end(); ++i)
			serverConfigInfo += "\n\t\tErrorPage[" + stp_itoa(i->first) + "].: " + i->second;
	}
	if (this->_routes.size() > 0) {
		serverConfigInfo += std::string("\n\tRoutes->");
		for (std::map<std::string, Route>::iterator i = this->_routes.begin(); i != this->_routes.end(); ++i)
			serverConfigInfo += "\n\t\tRoute[" + i->first + "]";
	}
	return (serverConfigInfo);
}
void			ServerConfig::initErrorPages(void) {
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
bool			ServerConfig::isValidHost(std::string host) const {
	struct sockaddr_in	sockaddr;
  	return (ws_inet_pton(AF_INET, host.c_str(), &(sockaddr.sin_addr)) ? true : false);
}
void			ServerConfig::setPort(const JsonNode& configJson) {
	try {
		this->_listen.port = configJson.TryGetInt("port");
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (ServerConfig::ErrorException("\"port\" Element should be integer!"));
		throw (ServerConfig::ErrorException(ex.what()));
	}
}
void			ServerConfig::setHost(const JsonNode& configJson) {
	try {
		this->setHost(configJson.TryGetString("host"));
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (ServerConfig::ErrorException("\"host\" Element should be string!"));
		throw (ServerConfig::ErrorException(ex.what()));
	}
}
void			ServerConfig::setServerName(const JsonNode& configJson) {
	try {
		this->setServerName(configJson.TryGetString("server_name"));
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (ServerConfig::ErrorException("\"server_name\" Element should be string!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			this->setServerName("Server");
		else
			throw (ServerConfig::ErrorException(ex.what()));
	}
}
void			ServerConfig::setDataDirectory(const JsonNode& configJson) {
	try {
		this->setDataDirectory(configJson.TryGetString("dataDirectory"));
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (ServerConfig::ErrorException("\"dataDirectory\" Element should be string!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			this->_dataDirectory = "./";
		else
			throw (ServerConfig::ErrorException(ex.what()));
	}
}
void			ServerConfig::setRoot(const JsonNode& configJson) {
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
void			ServerConfig::setIndex(const JsonNode& configJson) {
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
void			ServerConfig::setClientMaxBodySize(const JsonNode& configJson) {
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
void			ServerConfig::setAutoindex(const JsonNode& configJson) {
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
void			ServerConfig::setErrorPages(const JsonNode& configJson) {
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
void			ServerConfig::setRoutes(const JsonNode& configJson) {
	JsonChildren routesChildren;
	try {
		routesChildren = configJson.TryGetChildren("routes");
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (ServerConfig::ErrorException("\"routes\" Element should be children!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			return ;
		else
			throw (ServerConfig::ErrorException(ex.what()));
	}
	for (size_t i = 0; i < routesChildren.GetSize(); ++i) {
		Route route = Route(routesChildren.GetChildNode(i));
		this->_routes[route.getRoutePath()] = route;
	}
}
void			ServerConfig::parseErrorChild(const JsonNode& errorChild) {
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
std::string		ServerConfig::parseErrorElement(const JsonNode& errorChild, const std::string& element) {
	try {
		return (errorChild.TryGetString(element));
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (ServerConfig::ErrorException("\"error_pages." + element + "\" Element should be string!"));
		throw (ServerConfig::ErrorException(ex.what()));
	}
}
void			ServerConfig::deepCopy(const ServerConfig& src) {
	this->_listen = src._listen;
	this->_server_name = src._server_name;
	this->_dataDirectory = src._dataDirectory;
	this->_root = src._root;
	this->_index = src._index;
	this->_autoindex = src._autoindex;
	this->_client_max_body_size = src._client_max_body_size;
	this->_error_pages = src._error_pages;
	this->_routes = src._routes;
}