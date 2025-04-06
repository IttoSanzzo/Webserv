#include "webserv.hpp"

t_contentType	contentTypeFromString(const std::string& contentType) {
	if (contentType == ".html")
		return (textHtml);
	else if (contentType == ".css")
		return (textCss);
	else if (contentType == ".js")
		return (applicationJavascript);
	else if (contentType == ".png")
		return (imagePng);
	else if (contentType == ".jpeg")
		return (imageJpeg);
	else if (contentType == ".mp4")
		return (videoMp4);
	else if (contentType == ".svg")
		return (imageSvgXml);
	else if (contentType == ".zip")
		return (applicationZip);
	else if (contentType == "application/x-www-form-urlencoded")
		return (appXWwwFormUrlencoded);
	else if (contentType == "multipart/form-data")
		return (multipartFormData);
	else if (contentType == "text/plain")
		return (multipartFormData);
	else if (contentType == "folder")
		return (folder);
	else
		return (anyType);
}
std::string		contentTypeToString(const t_contentType& contentType) {
	switch (contentType) {
		case (textHtml):
			return ("text/html");
		break;
		case (textCss):
			return ("text/css");
		break;
		case (applicationJavascript):
			return ("application/javascript");
		break;
		case (imagePng):
			return ("image/png");
		break;
		case (imageJpeg):
			return ("image/jpeg");
		break;
		case (videoMp4):
			return ("video/mp4");
		break;
		case (imageSvgXml):
			return ("multipart/form-data");
		break;
		case (applicationZip):
			return ("application/zip");
		break;
		case (appXWwwFormUrlencoded):
			return ("application/x-www-form-urlencoded");
		break;
		case (multipartFormData):
			return ("multipart/form-data");
		break;
		case (textPlain):
			return ("text/plain");
		break;
		case (folder):
			return ("folder");
		break;
		default:
			return ("*/*");
		break;
	}
}
t_contentType	contentTypeFromFile(const std::string& filePath) {
	size_t	dotPos = filePath.rfind('.');
	if (dotPos == std::string::npos ) {
		if (filePath[filePath.size() - 1] == '/')
			return (folder);
		return (anyType);
	}
	return (contentTypeFromString(filePath.substr(dotPos)));
}
std::string		contentTypeToImage(const t_contentType& contentType) {
switch (contentType) {
		case (textHtml):
			return ("/server/html.png");
		break;
		case (textCss):
			return ("/server/css.png");
		break;
		case (applicationJavascript):
			return ("/server/javascript.png");
		break;
		case (imagePng):
			return ("/server/png.png");
		break;
		case (imageJpeg):
			return ("/server/jpeg.png");
		break;
		case (videoMp4):
			return ("/server/mp4.png");
		break;
		case (imageSvgXml):
			return ("/server/svg.png");
		break;
		case (applicationZip):
			return ("/server/zip.png");
		break;
		case (textPlain):
			return ("/server/txt.png");
		break;
		case (folder):
			return ("/server/folder.png");
		break;
		default:
			return ("/server/none.png");
		break;
	}
}