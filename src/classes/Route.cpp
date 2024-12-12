#include "webserv.hpp"

/* C. Constructors */
Route::~Route(void) {}
Route::Route(void) {
	this->_routePath = "";
	this->_index = "";
	this->_root = "";
	this->_returner = "";
	this->_autoindex = false;
}
Route::Route(const Route& src) {
	this->deepCopy(src);
}
Route::Route(const JsonNode& routeJson) {
	this->setRoutePath(routeJson);
	this->setIndex(routeJson);
	this->setRoot(routeJson);
	this->setReturner(routeJson);
	this->setAutoindex(routeJson);
	this->setMethods(routeJson);
	this->setCgiRelation(routeJson);
}
Route&	Route::operator=(const Route& src) {
	if (this != &src)
		this->deepCopy(src);
	return (*this);
}
void		Route::setRoutePath(const std::string& routePath) {
	this->_routePath = routePath;
}
void		Route::setIndex(const std::string& index) {
	this->_index = index;
}
void		Route::setRoot(const std::string& root) {
	this->_root = root;
}
void		Route::setAutoindex(const bool& value) {
	this->_autoindex = value;
}
void		Route::setMethods(const std::string& method, const bool& value) {
	this->_methods[method] = value;
}
void		Route::setCgiRelation(const std::string& extension, const std::string& path) {
	this->_cgiRelations[extension] = path;
}
std::string	Route::getRoutePath(void) const {
	return (this->_routePath);
}
std::string	Route::getIndex(void) const {
	return (this->_index);
}
std::string	Route::getRoot(void) const {
	return (this->_root);
}
std::string	Route::getReturner(void) const {
	return (this->_returner);
}
bool		Route::getAutoindex(void) const {
	return (this->_autoindex);
}
bool		Route::getMethod(const std::string& method) {
	return (this->_methods[method]);
}
std::string	Route::getCgiPath(const std::string& extension) {
	return (this->_cgiRelations[extension]);
}
void		Route::setRoutePath(const JsonNode& routeJson) {
	try {
		this->_routePath = routeJson.TryGetString("routePath");
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (Route::ErrorException("\"routes.routePath\" Element should be string!"));
		throw (Route::ErrorException(ex.what()));
	}
}
void		Route::setIndex(const JsonNode& routeJson) {
	try {
		this->_index = routeJson.TryGetString("index");
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (Route::ErrorException("\"routes.index\" Element should be string!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			this->_index = "";
		else
			throw (Route::ErrorException(ex.what()));
	}
}
void		Route::setRoot(const JsonNode& routeJson) {
	try {
		this->_root = routeJson.TryGetString("root");
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (Route::ErrorException("\"routes.root\" Element should be string!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			this->_root = "";
		else
			throw (Route::ErrorException(ex.what()));
	}
}
void		Route::setReturner(const JsonNode& routeJson) {
	try {
		this->_returner = routeJson.TryGetString("return");
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (Route::ErrorException("\"routes.return\" Element should be string!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			this->_returner = "";
		else
			throw (Route::ErrorException(ex.what()));
	}
}
void		Route::setAutoindex(const JsonNode& routeJson) {
	try {
		this->_autoindex = routeJson.TryGetBool("autoindex");
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (Route::ErrorException("\"routes.autoindex\" Element should be boolean!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			this->_autoindex = false;
		else
			throw (Route::ErrorException(ex.what()));
	}
}
void		Route::setMethods(const JsonNode& routeJson) {
	std::string	methodsString;
	try {
		methodsString = routeJson.TryGetString("allow_methods");
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (Route::ErrorException("\"routes.allow_methods\" Element should be string!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			return ;
		throw (Route::ErrorException(ex.what()));
	}
	std::string* allMethods = stp_split(methodsString, ' ');
	size_t	allMethodsSize = stp_stringArrayLength(allMethods);
	for (size_t i = 0; i < allMethodsSize; ++i)
		this->_methods[allMethods[i]] = true;
	delete[] allMethods;
}
void		Route::setCgiRelation(const JsonNode& routeJson) {
	JsonChildren cgiChildren;
	try {
		cgiChildren = routeJson.TryGetChildren("cgi");
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (Route::ErrorException("\"routes.cgi\" Element should be children!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			return ;
		else
			throw (Route::ErrorException(ex.what()));
	}
	for (size_t i = 0; i < cgiChildren.GetSize(); ++i)
		this->parseCgiChild(cgiChildren.GetChildNode(i));
}
void		Route::parseCgiChild(const JsonNode& cgiChild) {
	std::string	path = this->parseCgiElement(cgiChild, "path");
	std::string	extension = this->parseCgiElement(cgiChild, "extension");
	this->_cgiRelations[extension] = path;
}
std::string	Route::parseCgiElement(const JsonNode& cgiChild, const std::string& element) {
	try {
		return (cgiChild.TryGetString(element));
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (Route::ErrorException("\"error_routePaths." + element + "\" Element should be string!"));
		throw (Route::ErrorException(ex.what()));
	}
}
void		Route::deepCopy(const Route& src) {	
	this->_routePath = src._routePath;
	this->_index = src._index;
	this->_root = src._root;
	this->_returner = src._returner;
	this->_autoindex = src._autoindex;
	this->_methods = src._methods;
	this->_cgiRelations = src._cgiRelations;
}