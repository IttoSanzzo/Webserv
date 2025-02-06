#include "webserv.hpp"

HttpResponse::~HttpResponse(void) {}
HttpResponse::HttpResponse(void) {
	this->_code = 0;
	this->_contentType = anyType;
	this->_content = "";
	this->_keep_alive = false;
	this->_redirect = false;
}
HttpResponse::HttpResponse(const HttpResponse& src) {
	this->deepCopy(src);
}
HttpResponse&	HttpResponse::operator=(const HttpResponse& src) {
	if (this != &src)
		this->deepCopy(src);
	return (*this);
}
void			HttpResponse::setCode(const short& code) {
	this->_code = code;
}
void			HttpResponse::setType(const t_contentType& type) {
	this->_contentType = type;
}
void			HttpResponse::setContent(const std::string& content) {
	this->_content = content;
}
void			HttpResponse::setKeepAlive(const bool& keepAlive) {
	this->_keep_alive = keepAlive;
}
void			HttpResponse::setRedirect(const bool& redirect) {
	this->_redirect = redirect;
}
short			HttpResponse::getCode(void) const {
	return (this->_code);
}
t_contentType	HttpResponse::getType(void) const {
	return (this->_contentType);
}
size_t			HttpResponse::getHeaderLength(void) const {
	return (this->getHeader().size());
}
size_t			HttpResponse::getContentLength(void) const {
	return (this->_content.size());
}
std::string		HttpResponse::getContent(void) const {
	return (this->_content);
}
bool			HttpResponse::getKeepAlive(void) const {
	return (this->_keep_alive);
}
bool			HttpResponse::getRedirect(void) const {
	return (this->_redirect);
}
size_t			HttpResponse::getSize(void) const {
	return (this->toString().size());
}
std::string		HttpResponse::getHeader(void) const {
	if (this->_redirect == true)
		return ("HTTP/1.1 301 Moved Permanently\r\nLocation: " + this->_content +  "\r\nContent-Length: 0\r\nConnection: close\r\n\r\n");
	std::string	header = "HTTP/1.1 ";
	header += stp_itoa(this->_code);
	std::string	codeDescription = httpStatusCodeToString(this->_code);
	if (codeDescription != "")
		header += " " + codeDescription;
	header += "\r\nContent-Type: " + contentTypeToString(this->_contentType);
	header += "\r\nContent-Length: " + stp_itoa(this->getContentLength());
	if (this->_keep_alive == true)
		header += "\r\nConnection: keep-alive\r\nKeep-Alive: timeout=" + stp_itoa(CLIENTTIMEOUT);
	return (header);
}
std::string		HttpResponse::getFullHeader(void) const {
	return (this->getHeader() + "\r\n\r\n");
}
std::string		HttpResponse::toString(void) const {
	if (this->_redirect == true)
		return ("HTTP/1.1 301 Moved Permanently\r\nLocation: " + this->_content +  "\r\nContent-Length: 0\r\nConnection: close\r\n\r\n");
	return (this->getHeader() + "\r\n\r\n" + this->getContent());
}
void			HttpResponse::doRedirectResponse(const std::string& target) {
	this->_code = 301;
	this->_redirect = true;
	this->_content = target;
}
void			HttpResponse::deepCopy(const HttpResponse& src) {
	this->_code = src._code;
	this->_contentType = src._contentType;
	this->_content = src._content;
	this->_keep_alive = src._keep_alive;
	this->_redirect = src._redirect;
}