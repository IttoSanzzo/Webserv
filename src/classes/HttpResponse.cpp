#include "webserv.hpp"

HttpResponse::~HttpResponse(void) {}
HttpResponse::HttpResponse(void) {
	this->_code = 0;
	this->_contentType = typeless;
	this->_content = "";
}
HttpResponse::HttpResponse(const HttpResponse& src) {
	this->deepCopy(src);
}
HttpResponse	HttpResponse::operator=(const HttpResponse& src) {
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
short			HttpResponse::getCode(void) const {
	return (this->_code);
}
std::string		HttpResponse::getCodeDescription(void) const {
	return (this->getCodeDescription(this->_code));
}
std::string		HttpResponse::getCodeDescription(const short& code) const {
	switch (code) {
		case (200):
			return ("OK");
		break;
		case (404):
			return ("Not Found");
		break;
		default:
			return ("");
		break;
	}
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
size_t			HttpResponse::getSize(void) const {
	return (this->toString().size());
}
std::string		HttpResponse::getHeader(void) const {
	std::string	header = "HTTP/1.1 ";
	header += stp_itoa(this->_code);
	std::string	codeDescription = this->getCodeDescription();
	if (codeDescription != "")
		header += " " + codeDescription;
	header += "\r\nContent-Type: " + HttpResponse::contentTypeString(this->_contentType);
	header += "\r\nContent-Length: " + stp_itoa(this->getContentLength());
	return (header);
}
std::string		HttpResponse::toString(void) const {
	return (this->getHeader() + "\r\n\r\n" + this->getContent());
}
std::string		HttpResponse::contentTypeString(const t_contentType& type) {
	switch (type) {
		case (textHtml):
			return ("text/html");
		break;
		case (textCss):
			return ("text/css");
		break;
		default:
			return ("");
		break;
	}
}
void			HttpResponse::deepCopy(const HttpResponse& src) {
	this->_code = src._code;
	this->_contentType = src._contentType;
	this->_content = src._content;
}