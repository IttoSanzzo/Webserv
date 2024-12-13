#include "webserv.hpp"

RequestProcessor::~RequestProcessor(void) {}
RequestProcessor::RequestProcessor(void) {}
RequestProcessor::RequestProcessor(const HttpRequest& request, Server* server) {
	this->_server = server;
	this->_request = request;
	this->_response = HttpResponse();
}
RequestProcessor::RequestProcessor(const RequestProcessor& src) {
	this->deepCopy(src);
}
RequestProcessor&	RequestProcessor::operator=(const RequestProcessor& src) {
	if (this != &src)
		this->deepCopy(src);
	return (*this);
}
void				RequestProcessor::setServer(Server* server) {
	this->_server = server;
}
void				RequestProcessor::setRequest(const HttpRequest& request) {
	this->_request = request;
}
short				RequestProcessor::process(const int& socketFd) {
	switch (this->_request.getMethodType()) {
		case (GET):
			this->getMethod(socketFd);
		break;
		case (POST):
			this->postMethod(socketFd);
		break;
		case (PUT):
			this->putMethod(socketFd);
		break;
		case (PATCH):
			this->patchMethod(socketFd);
		break;
		case (DELETE):
			this->deleteMethod(socketFd);
		break;
		case (HEAD):
			this->headMethod(socketFd);
		break;
		case (OPTIONS):
			this->optionsMethod(socketFd);
		break;
		default:
		break;
	}
	if (this->_response.getCode() != 200)
		this->doErrorPage();
	std::string	completeResponse(this->_response.toString());
	::send(socketFd, completeResponse.c_str(), completeResponse.size(), 0);
	::close(socketFd);
	return (0);
}
HttpResponse		RequestProcessor::readFileToResponse(const std::string& filePath) {
	HttpResponse		htmlResponse;
	std::ofstream		file;
	std::stringstream	buffer;
	std::string			finalFilePath("./" + this->_server->getServerConfig().getDataDirectory() + std::string("/") + filePath);
	if (this->fileExists(finalFilePath)) {
		file.open(finalFilePath.c_str(), std::ifstream::in);
		if (file.is_open() == false)
			htmlResponse.setCode(403);
		else {
			htmlResponse.setType(contentTypeFromFile(filePath));
			buffer << file.rdbuf();
			file.close();
			htmlResponse.setCode(200);
			htmlResponse.setContent(buffer.str());
		}
	}
	else
		htmlResponse.setCode(404);
	return (htmlResponse);
}
int					RequestProcessor::fileExists(std::string filePath) {
	struct stat	stats;
	if (stat(filePath.c_str(), &stats) == 0)
		return (1);
	return (0);
}
void				RequestProcessor::deepCopy(const RequestProcessor& src) {
	this->_server = src._server;
	this->_request = src._request;
	this->_response = src._response;
}
void				RequestProcessor::doErrorPage(void) {
	Log::log("Doing Error Page " + stp_itoa(this->_response.getCode()));
	this->_response.setType(textHtml);
	HttpResponse	errorResponse;
	std::string		errorPagePath(this->_server->getServerConfig().getErrorPage(this->_response.getCode()));
	if (errorPagePath != "") {
		errorResponse = this->readFileToResponse(errorPagePath);
		if (errorResponse.getCode() == 200) {
			errorResponse.setCode(this->_response.getCode());
			this->_response = errorResponse;
			return ;
		}
		errorResponse.setCode(this->_response.getCode());
		errorResponse.setContent(ERRORPAGEERROR);
		this->_response = errorResponse;
		return ;
	}
	errorResponse.setCode(this->_response.getCode());
	errorResponse.setContent(ERRORPAGE);
	this->_response = errorResponse;
}
void				RequestProcessor::getMethod(const int& socketFd) {
	Log::debug(std::string("TargetRoute is: |") + this->_request.getTargetRoute() + std::string("|"));
	Route		route;
	std::string	index;
	if (this->_request.getReferer() != "") {
		Log::debug("Referer: " + this->_request.getReferer());
		std::string refererTargetRoute(this->_request.getReferer().substr(this->_request.getReferer().find(this->_server->getServerConfig().getListen().toString()) + this->_server->getServerConfig().getListen().toString().length()));
		route = this->getRoute(refererTargetRoute);
		index = this->_request.getTargetRoute();
	}
	else {
		route = this->getRoute(this->_request.getTargetRoute());
		route = this->getRoute(this->_request.getTargetRoute());
		index = route.getIndex();
	}
	Log::debug("RoutePath: " + route.getRoutePath());
	Log::debug("Index: " + index);
	if (route.getMethod("GET") == false) {
		this->_response.setCode(403);
		return ;
	}
	this->_response = this->readFileToResponse(index);
	if (this->_response.getCode() != 200)
		return ;
	(void)socketFd;
}
void				RequestProcessor::postMethod(const int& socketFd) {
	(void)socketFd;
}
void				RequestProcessor::putMethod(const int& socketFd) {
	(void)socketFd;
}
void				RequestProcessor::patchMethod(const int& socketFd) {
	(void)socketFd;
}
void				RequestProcessor::deleteMethod(const int& socketFd) {
	(void)socketFd;
}
void				RequestProcessor::headMethod(const int& socketFd) {
	(void)socketFd;
}
void				RequestProcessor::optionsMethod(const int& socketFd) {
	(void)socketFd;
}
Route				RequestProcessor::getRoute(const std::string& route) {
	Log::debug("Getting TargetedRoute: |" + route + "|");
	return (this->_server->getServerConfig().getRoute(route));
}