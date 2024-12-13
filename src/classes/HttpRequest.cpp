#include "webserv.hpp"

HttpRequest::~HttpRequest(void) {}
HttpRequest::HttpRequest(void) {
	this->_originalString = "";
	this->_method = no_method;
	this->_targetRoute = "";
	this->_host = "";
	this->_protocol = no_protocol;
	this->_userAgent = "";
}
HttpRequest::HttpRequest(const HttpRequest& src) {
	this->deepCopy(src);

}
HttpRequest::HttpRequest(const std::string& request) {
	this->_originalString = "";
	this->_method = no_method;
	this->_targetRoute = "";
	this->_host = "";
	this->_protocol = no_protocol;
	this->_userAgent = "";
	this->_referer = "";
	std::vector<std::string> lines = stp_split(request, "\n");
	std::vector<std::string> mainParts = stp_split(lines[0], " ");
	this->_method = methodFromString(mainParts[0]);
	this->_targetRoute = mainParts[1];
	this->_protocol = protocolFromString(mainParts[2]);
	for (size_t i = 1; i < lines.size(); ++i) {
		if (lines[i] == "\r")
			continue;
		std::vector<std::string> parts = stp_split(lines[i], ": ");
		this->setSwitch(parts[0], parts[1]);
	}
}
HttpRequest&	HttpRequest::operator=(const HttpRequest& src) {
	if (this != &src)
		this->deepCopy(src);
	return (*this);
}
void			HttpRequest::setSwitch(const std::string& name, const std::string& value) {
	if (name == "Host")
		this->_host = value;
	else if (name == "User-Agent")
		this->_userAgent = value;
	else if (name == "Accept")
		this->_accept.push_back(value);
	else if (name == "Referer") {
		this->_referer = value;
		this->_referer.erase(value.find('\r'));
	}
	else if (name == "Accept-Encoding")
		this->_acceptEncoding = stp_split(value, ", ");
	else
		this->_others[name] = value;
}
void			HttpRequest::setOriginalString(const std::string& src) {
	this->_originalString = src;
}
void			HttpRequest::setMethodType(const t_method& method) {
	this->_method = method;
}
void			HttpRequest::setTargetRoute(const std::string& targetRoute) {
	this->_targetRoute = targetRoute;
}
void			HttpRequest::setHost(const std::string& host) {
	this->_host = host;
}
void			HttpRequest::setProtocol(const t_protocol& protocol) {
	this->_protocol = protocol;
}
void			HttpRequest::setUserAgent(const std::string& userAgent) {
	this->_userAgent = userAgent;
}
void			HttpRequest::setReferer(const std::string& referer) {
	this->_referer = referer;
}
void			HttpRequest::pushAccept(const std::string& accept) {
	this->_accept.push_back(accept);
}
void			HttpRequest::pushAcceptEncoding(const std::string& acceptEncoding) {
	this->_acceptEncoding.push_back(acceptEncoding);

}
std::string		HttpRequest::getOriginalString(void) const {
	return (this->_originalString);
}
t_method		HttpRequest::getMethodType(void) const {
	return (this->_method);
}
std::string		HttpRequest::getTargetRoute(void) const {
	return (this->_targetRoute);
}
std::string		HttpRequest::getHost(void) const {
	return (this->_host);
}
t_protocol		HttpRequest::getProtocol(void) const {
	return (this->_protocol);
}
std::string		HttpRequest::getUserAgent(void) const {
	return (this->_userAgent);
}
std::string		HttpRequest::getReferer(void) const {
	return (this->_referer);
}
std::string		HttpRequest::getAccept(const size_t& pos) const {
	return (this->_accept[pos]);
}
std::string		HttpRequest::getAcceptEncoding(const size_t& pos) const {
	return (this->_acceptEncoding[pos]);
}
std::string		HttpRequest::getOther(const std::string& name) {
	return (this->_others[name]);
}
std::map<std::string, std::string>&	HttpRequest::getOther(void) {
	return (this->_others);
}
std::string		HttpRequest::toString(void) {
	std::string	returnString = "Method: " + methodToString(this->getMethodType()) + "\n";
	returnString += "Route: " + this->getTargetRoute() + "\n";
	returnString += "Protocol: " + protocolToString(this->getProtocol()) + "\n";
	returnString += "Host: " + this->getHost() + "\n";
	returnString += "Agent: " + this->getUserAgent();
	for (size_t i = 0; i < this->_accept.size(); ++i)
		returnString += "\n" + std::string("Accept: ") + this->getAccept(i);
	for (size_t i = 0; i < this->_acceptEncoding.size(); ++i)
		returnString += "\n" + std::string("AcceptEncoding: ") + this->getAcceptEncoding(i);
	for (std::map<std::string, std::string>::iterator i = this->_others.begin(); i != this->_others.end(); ++i)
		returnString += "\n" + i->first + std::string(": ") + i->second;
	return (returnString);
}
void			HttpRequest::deepCopy(const HttpRequest& src) {
	this->_originalString = src._originalString;
	this->_method = src._method;
	this->_targetRoute = src._targetRoute;
	this->_host = src._host;
	this->_protocol = src._protocol;
	this->_userAgent = src._userAgent;
	this->_referer = src._referer;
	this->_accept = src._accept;
	this->_acceptEncoding = src._acceptEncoding;
	this->_others = src._others;
}