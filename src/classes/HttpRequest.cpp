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
	std::vector<std::string> lines = stp_split(request, "\n");
	std::vector<std::string> mainParts = stp_split(lines[0], " ");
	this->_method = HttpRequest::methodFromString(mainParts[0]);
	this->_targetRoute = mainParts[1];
	this->_protocol = HttpRequest::protocolFromString(mainParts[2]);
	for (size_t i = 1; i < lines.size(); ++i) {
		if (lines[i] == "\r")
			continue;
		std::vector<std::string> parts = stp_split(lines[i], ": ");
		this->setSwitch(parts[0], parts[1]);
	}
}
HttpRequest		HttpRequest::operator=(const HttpRequest& src) {
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
	else if (name == "Accept-Encoding")
		this->_acceptEncoding = stp_split(value, ", ");
	else
		this->_others[name] = value;
}
void			HttpRequest::setOriginalString(const std::string& src) {
	this->_originalString = src;
}
void			HttpRequest::setMethod(const t_method& method) {
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
void			HttpRequest::pushAccept(const std::string& accept) {
	this->_accept.push_back(accept);
}
void			HttpRequest::pushAcceptEncoding(const std::string& acceptEncoding) {
	this->_acceptEncoding.push_back(acceptEncoding);

}
std::string		HttpRequest::getOriginalString(void) const {
	return (this->_originalString);
}
t_method		HttpRequest::getMethod(void) const {
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
std::string		HttpRequest::getAccept(const size_t& pos) const {
	return (this->_accept[pos]);
}
std::string		HttpRequest::getAcceptEncoding(const size_t& pos) const {
	return (this->_acceptEncoding[pos]);
}
t_method		HttpRequest::methodFromString(const std::string& method) {
	if (method == "GET")
		return (GET);
	else if (method == "POST")
		return (POST);
	else if (method == "PUT")
		return (PUT);
	else if (method == "PATCH")
		return (PATCH);
	else if (method == "DELETE")
		return (DELETE);
	else if (method == "HEAD")
		return (HEAD);
	else if (method == "OPTIONS")
		return (OPTIONS);
	else
		return (no_method);
}
std::string		HttpRequest::methodToString(const t_method& method) {
	switch (method) {
		case (GET):
			return ("GET");
		break;
		case (POST):
			return ("POST");
		break;
		case (PUT):
			return ("PUT");
		break;
		case (PATCH):
			return ("PATCH");
		break;
		case (DELETE):
			return ("DELETE");
		break;
		case (HEAD):
			return ("HEAD");
		break;
		case (OPTIONS):
			return ("OPTIONS");
		break;
		default:
			return ("");
		break;
	}
}
t_protocol		HttpRequest::protocolFromString(const std::string& protocol) {
	if (protocol == "HTTP/1.1")
		return (http1dot1);
	else
		return (no_protocol);
}
std::string		HttpRequest::protocolToString(const t_protocol& protocol) {
	switch (protocol) {
		case (http1dot1):
			return ("HTTP/1.1");
		break;
		default:
			return ("");
		break;
	}
}
std::string		HttpRequest::getOther(const std::string& name) {
	return (this->_others[name]);
}
std::map<std::string, std::string>&	HttpRequest::getOther(void) {
	return (this->_others);
}
void			HttpRequest::deepCopy(const HttpRequest& src) {
	this->_originalString = src._originalString;
	this->_method = src._method;
	this->_targetRoute = src._targetRoute;
	this->_host = src._host;
	this->_protocol = src._protocol;
	this->_userAgent = src._userAgent;
	this->_accept = src._accept;
	this->_acceptEncoding = src._acceptEncoding;
	this->_others = src._others;
}