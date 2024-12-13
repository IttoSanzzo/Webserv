#include "webserv.hpp"

t_contentType	contentTypeFromString(const std::string& contentType) {
	if (contentType == ".html")
		return (textHtml);
	else if (contentType == ".css")
		return (textCss);
	else if (contentType == ".js")
		return (applicationJavascript);
	else
		return (anyType);
}
std::string		contentTypeToString(const t_contentType& contentType) {
	switch (contentType) {
		case (textHtml):
			return ("text/html");
		break;
		case (textCss):
			return ("text.css");
		break;
		case (applicationJavascript):
			return ("application/javascript");
		break;
		default:
			return ("*/*");
		break;
	}
}
t_contentType	contentTypeFromFile(const std::string& filePath) {
	size_t	dotPos = filePath.rfind('.');
	if (dotPos == std::string::npos)
		return (anyType);
	return (contentTypeFromString(filePath.substr(dotPos)));
}