#include "webserv.hpp"

/* C. Constructors */
Location::~Location(void) {}
Location::Location(void) {
	this->_page = "";
	this->_index = "";
	this->_root = "";
	this->_returner = "";
	this->_autoindex = false;
}
Location::Location(const Location& src) {
	this->deepCopy(src);
}
Location&	Location::operator=(const Location& src) {
	if (this != &src)
		this->deepCopy(src);
	return (*this);
}
Location::Location(const JsonNode& locationJson) {
	this->setPage(locationJson);
	this->setIndex(locationJson);
	this->setRoot(locationJson);
	this->setReturner(locationJson);
	this->setAutoindex(locationJson);
	this->setMethods(locationJson);
	this->setCgiRelation(locationJson);
}

/* S. Setters */
void	Location::setPage(const std::string& page) {
	this->_page = page;
}
void	Location::setIndex(const std::string& index) {
	this->_index = index;
}
void	Location::setRoot(const std::string& root) {
	this->_root = root;
}
void	Location::setReturner(const std::string& target) {
	this->_returner = target;
}
void	Location::setAutoindex(const bool& value) {
	this->_autoindex = value;
}
void	Location::setMethods(const std::string& method, const bool& value) {
	this->_methods[method] = value;
}
void	Location::setCgiRelation(const std::string& extension, const std::string& path) {
	this->_cgiRelations[extension] = path;
}

/* G. Getters */
std::string	Location::getPage(void) const {
	return (this->_page);
}
std::string	Location::getIndex(void) const {
	return (this->_index);
}
std::string	Location::getRoot(void) const {
	return (this->_root);
}
std::string	Location::getReturner(void) const {
	return (this->_returner);
}
bool		Location::getAutoindex(void) const {
	return (this->_autoindex);
}
bool		Location::getMethod(const std::string& method) {
	return (this->_methods[method]);
}
std::string	Location::getCgiPath(const std::string& extension) {
	return (this->_cgiRelations[extension]);
}

/* PRI0. JsonParsing */
void	Location::setPage(const JsonNode& locationJson) {
	try {
		this->_page = locationJson.TryGetString("page");
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (Location::ErrorException("\"locations.page\" Element should be string!"));
		throw (Location::ErrorException(ex.what()));
	}
}
void	Location::setIndex(const JsonNode& locationJson) {
	try {
		this->_index = locationJson.TryGetString("index");
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (Location::ErrorException("\"locations.index\" Element should be string!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			this->_index = "";
		else
			throw (Location::ErrorException(ex.what()));
	}
}
void	Location::setRoot(const JsonNode& locationJson) {
	try {
		this->_root = locationJson.TryGetString("root");
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (Location::ErrorException("\"locations.root\" Element should be string!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			this->_root = "";
		else
			throw (Location::ErrorException(ex.what()));
	}
}
void	Location::setReturner(const JsonNode& locationJson) {
	try {
		this->_returner = locationJson.TryGetString("return");
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (Location::ErrorException("\"locations.return\" Element should be string!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			this->_returner = "";
		else
			throw (Location::ErrorException(ex.what()));
	}
}
void	Location::setAutoindex(const JsonNode& locationJson) {
	try {
		this->_autoindex = locationJson.TryGetBool("autoindex");
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (Location::ErrorException("\"locations.autoindex\" Element should be boolean!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			this->_autoindex = false;
		else
			throw (Location::ErrorException(ex.what()));
	}
}
void	Location::setMethods(const JsonNode& locationJson) {
	std::string	methodsString;
	try {
		methodsString = locationJson.TryGetString("allow_methods");
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (Location::ErrorException("\"locations.allow_methods\" Element should be string!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			return ;
		throw (Location::ErrorException(ex.what()));
	}
	std::string* allMethods = stp_split(methodsString, ' ');
	size_t	allMethodsSize = stp_stringArrayLength(allMethods);
	for (size_t i = 0; i < allMethodsSize; ++i)
		this->_methods[allMethods[i]] = true;
	delete[] allMethods;
}
void	Location::setCgiRelation(const JsonNode& locationJson) {
	JsonChildren cgiChildren;
	try {
		cgiChildren = locationJson.TryGetChildren("cgi");
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (Location::ErrorException("\"locations.cgi\" Element should be children!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			return ;
		else
			throw (Location::ErrorException(ex.what()));
	}
	for (size_t i = 0; i < cgiChildren.GetSize(); ++i)
		this->parseCgiChild(cgiChildren.GetChildNode(i));
}
void		Location::parseCgiChild(const JsonNode& cgiChild) {
	std::string	path = this->parseCgiElement(cgiChild, "path");
	std::string	extension = this->parseCgiElement(cgiChild, "extension");
	this->_cgiRelations[extension] = path;
}
std::string	Location::parseCgiElement(const JsonNode& cgiChild, const std::string& element) {
	try {
		return (cgiChild.TryGetString(element));
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (Location::ErrorException("\"error_pages." + element + "\" Element should be string!"));
		throw (Location::ErrorException(ex.what()));
	}
}

/* PRIU. Utils */
void	Location::deepCopy(const Location& src) {
	this->_page = src._page;
	this->_index = src._index;
	this->_root = src._root;
	this->_returner = src._returner;
	this->_autoindex = src._autoindex;
	this->_methods = src._methods;
	this->_cgiRelations = src._cgiRelations;
}