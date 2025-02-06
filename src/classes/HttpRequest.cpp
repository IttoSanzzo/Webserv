#include "webserv.hpp"

HttpRequest::~HttpRequest(void) {}
HttpRequest::HttpRequest(void) {
	this->_originalString = "";
	this->_method = no_method;
	this->_targetRoute = "";
	this->_host = "";
	this->_protocol = no_protocol;
	this->_userAgent = "";
	this->_contentLength = 0;
	this->_body = "";
	this->_fullCookie = "";
}
HttpRequest::HttpRequest(const HttpRequest& src) {
	this->deepCopy(src);
}
HttpRequest::HttpRequest(const std::string& request) {
	this->_originalString = request;
	this->_method = no_method;
	this->_targetRoute = "";
	this->_host = "";
	this->_protocol = no_protocol;
	this->_userAgent = "";
	this->_contentLength = 0;
	this->_body = "";
	this->_fullCookie = "";
	this->setHeaderPart(request.substr(0, request.find("\r\n\r") + 1));
	this->setBody(request.substr(request.find("\r\n\r") + 4));
	this->doQueryParameters(this->getTargetRoute());
}
HttpRequest&	HttpRequest::operator=(const HttpRequest& src) {
	if (this != &src)
		this->deepCopy(src);
	return (*this);
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
void			HttpRequest::setContentLength(const size_t& contentLength) {
	this->_contentLength = contentLength;
}
void			HttpRequest::setBody(const std::string& body) {
	this->_body = body;
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
size_t			HttpRequest::getContentLength(void) const {
	return (this->_contentLength);
}
std::string		HttpRequest::getBody(void) {
	return (this->_body);
}
std::string		HttpRequest::getFullCookie(void) const {
	return (this->_fullCookie);
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
std::string		HttpRequest::getCookie(const std::string& name) {
	return (this->_cookies[name]);
}
std::string		HttpRequest::getQueryParameter(const std::string& name) {
	return (this->_queryParameters[name]);
}
std::map<std::string, std::string>&	HttpRequest::getQueryParameters(void) {
	return (this->_queryParameters);
}
std::map<std::string, std::string>&	HttpRequest::getOther(void) {
	return (this->_others);
}
void			HttpRequest::setHeaderPart(const std::string& header) {
	std::vector<std::string> lines = stp_split(header, "\n");
	if (lines.size() == 0)
		return ;
	std::vector<std::string> mainParts = stp_split(lines[0], " ");
	this->_method = methodFromString(mainParts[0]);
	this->_targetRoute = mainParts[1];
	this->_protocol = protocolFromString(mainParts[2]);
	for (size_t i = 1; i < lines.size(); ++i) {
		std::vector<std::string> parts = stp_split(lines[i], ": ");
		this->setHeaderSwitch(parts[0], std::string(parts[1]).erase(parts[1].rfind('\r')));
	}
}
void			HttpRequest::setHeaderSwitch(const std::string& name, const std::string& value) {
	if (name == "Host")
		this->_host = value;
	else if (name == "User-Agent")
		this->_userAgent = value;
	else if (name == "Accept")
		this->_accept.push_back(value);
	else if (name == "Accept-Encoding")
		this->_acceptEncoding = stp_split(value, ", ");
	else if (name == "Content-Length")
		this->_contentLength = std::atoi(value.c_str());
	else if (name == "Cookie") {
		this->_fullCookie = value;
		std::vector<std::string> individualCookies = stp_split(value, "; ");
		for (size_t i = 0; i < individualCookies.size(); ++i) {
			std::vector<std::string> cookie = stp_split(individualCookies[i], "=");
			this->_cookies[cookie[0]] = cookie[1];
		}
	}
	else
		this->_others[name] = value;
}
void			HttpRequest::doQueryParameters(const std::string& fullRoute) {
	size_t	questionMark = fullRoute.find('?');
	if (questionMark == std::string::npos)
		return ;
	this->_targetRoute = this->_targetRoute.substr(0, questionMark);
	std::vector<std::string> fullQueryParameters = stp_split(fullRoute.substr(questionMark + 1), "&");
	std::vector<std::string> queryPar;
	for (size_t i = 0; i < fullQueryParameters.size(); ++i) {
		queryPar = stp_split(fullQueryParameters[i], "=");
		if (queryPar.size() > 1)
			this->_queryParameters[queryPar[0]] = queryPar[1];
	}
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
	for (std::map<std::string, std::string>::iterator i = this->_cookies.begin(); i != this->_cookies.end(); ++i)
		returnString += "\nCookie: " + i->first + std::string("=") + i->second;
	for (std::map<std::string, std::string>::iterator i = this->_queryParameters.begin(); i != this->_queryParameters.end(); ++i)
		returnString += "\nQuery |" + i->first + std::string("=") + i->second + std::string("|");
	return (returnString);
}
void			HttpRequest::deepCopy(const HttpRequest& src) {
	this->_originalString = src._originalString;
	this->_method = src._method;
	this->_targetRoute = src._targetRoute;
	this->_host = src._host;
	this->_protocol = src._protocol;
	this->_userAgent = src._userAgent;
	this->_contentLength = src._contentLength;
	this->_accept = src._accept;
	this->_acceptEncoding = src._acceptEncoding;
	this->_body = src._body;
	this->_fullCookie = src._fullCookie;
	this->_others = src._others;
	this->_cookies = src._cookies;
	this->_queryParameters = src._queryParameters;
}