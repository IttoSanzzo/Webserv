#include "webserv.hpp"

Route::~Route(void) {}
Route::Route(void) {
	this->_routePath = "";
	this->_index = "";
	this->_redirect = "";
	this->_savePath = "";
	this->_autoindex = false;
	this->_cgi = false;
}
Route::Route(const Route& src) {
	this->deepCopy(src);
}
Route::Route(const JsonNode& routeJson) {
	this->setRoutePath(routeJson);
	this->setIndex(routeJson);
	this->setRedirect(routeJson);
	this->setSavePath(routeJson);
	this->setAutoindex(routeJson);
	this->setCgi(routeJson);
	this->setMethods(routeJson);
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
void		Route::setSavePath(const std::string& savePath) {
	this->_savePath = savePath;
}
void		Route::setAutoindex(const bool& value) {
	this->_autoindex = value;
}
void		Route::setCgi(const bool& value) {
	this->_cgi = value;
}
void		Route::setMethods(const std::string& method, const bool& value) {
	this->_methods[method] = value;
}
std::string	Route::getRoutePath(void) const {
	return (this->_routePath);
}
std::string	Route::getIndex(void) const {
	return (this->_index);
}
std::string	Route::getRedirect(void) const {
	return (this->_redirect);
}
std::string	Route::getSavePath(void) const {
	return (this->_savePath);
}
bool		Route::getAutoindex(void) const {
	return (this->_autoindex);
}
bool		Route::getCgi(void) const {
	return (this->_cgi);
}
bool		Route::getMethod(const t_method& method) {
	return (this->_methods[methodToString(method)]);
}
bool		Route::getMethod(const std::string& method) {
	return (this->_methods[method]);
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
void		Route::setRedirect(const JsonNode& routeJson) {
	try {
		this->_redirect = routeJson.TryGetString("redirect");
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (Route::ErrorException("\"routes.redirect\" Element should be string!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			this->_redirect = "";
		else
			throw (Route::ErrorException(ex.what()));
	}
}
void		Route::setSavePath(const JsonNode& routeJson) {
	try {
		this->_savePath = routeJson.TryGetString("savePath");
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (Route::ErrorException("\"routes.savePath\" Element should be string!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			this->_savePath = "";
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
void		Route::setCgi(const JsonNode& routeJson) {
	try {
		this->_cgi = routeJson.TryGetBool("cgi");
	} catch (const std::exception& ex) {
		if (std::string(ex.what()).find("Not this type") != std::string::npos)
			throw (Route::ErrorException("\"routes.cgi\" Element should be boolean!"));
		else if (std::string(ex.what()).find("Not Found") != std::string::npos)
			this->_cgi = false;
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
void		Route::deepCopy(const Route& src) {	
	this->_routePath = src._routePath;
	this->_index = src._index;
	this->_redirect = src._redirect;
	this->_savePath = src._savePath;
	this->_autoindex = src._autoindex;
	this->_cgi = src._cgi;
	this->_methods = src._methods;
}