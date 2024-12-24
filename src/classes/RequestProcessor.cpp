#include "webserv.hpp"

RequestProcessor::~RequestProcessor(void) {}
RequestProcessor::RequestProcessor(void) {}
RequestProcessor::RequestProcessor(const HttpRequest& request, Server* server, const int& clientFD) {
	this->_clientFD = clientFD;
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
void				RequestProcessor::setClientFD(const int& clientFD) {
	this->_clientFD = clientFD;
}
void				RequestProcessor::setServer(Server* server) {
	this->_server = server;
}
void				RequestProcessor::setRequest(const HttpRequest& request) {
	this->_request = request;
}
bool				RequestProcessor::process(void) {
	switch (this->_request.getMethodType()) {
		case (GET):
			this->getMethod();
		break;
		case (POST):
			this->postMethod();
		break;
		case (PUT):
			this->putMethod();
		break;
		case (PATCH):
			this->patchMethod();
		break;
		case (DELETE):
			this->deleteMethod();
		break;
		case (HEAD):
			this->headMethod();
		break;
		case (OPTIONS):
			this->optionsMethod();
		break;
		default:
		break;
	}
	if (this->_response.getCode() != 200 && this->_response.getCode() != 301)
		this->doErrorPage();
	if (this->_request.getOther("Connection") == "keep-alive" && this->_response.getCode() != 301)
		this->_response.setKeepAlive(true);
	this->sendResponse(this->_response.toString());
	return (this->_response.getKeepAlive());
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
	this->_clientFD = src._clientFD;
	this->_server = src._server;
	this->_request = src._request;
	this->_response = src._response;
}
void				RequestProcessor::doErrorPage(void) {
	Log::log("Doing Error Page [" + stp_itoa(this->_response.getCode()) + std::string("]"));
	this->_response.setType(textHtml);
	std::string		errorPagePath(this->_server->getServerConfig().getErrorPage(this->_response.getCode()));
	if (errorPagePath != "") {
		HttpResponse errorResponse = this->readFileToResponse(errorPagePath);
		if (errorResponse.getCode() != 200)
			errorResponse.setContent(ERRORPAGEERROR);
		errorResponse.setCode(this->_response.getCode());
		this->_response = errorResponse;
		return ;
	}
	std::string	manualErroContent = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\" /><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" /><title>Error ";
	manualErroContent += stp_itoa(this->_response.getCode()) + std::string("</title><link rel=\"stylesheet\" href=\"/AllErrorPages/error.css\" /></head><body><div id=\"error\"><div id=\"errorCode\">Error ");
	manualErroContent += stp_itoa(this->_response.getCode()) + std::string("</div><div id=\"errorMessage\">");
	manualErroContent += httpStatusCodeToString(this->_response.getCode()) + std::string("!</div></div></body></html>");
	this->_response.setContent(manualErroContent);
}
void				RequestProcessor::getMethod() {
	Route route = this->resolveRoute(this->_request.getTargetRoute());
	if (route.getRoutePath() == "") {
		this->_response.setCode(404);
		return ;
	}
	else if (route.getMethod("GET") == false) {
		this->_response.setCode(403);
		return ;
	}
	if (route.getRedirect() != "") {
		if (route.getRedirect().find("http") == std::string::npos)
			this->_response.doRedirectResponse(std::string("http://") + this->_server->getServerConfig().getListen().toString() + route.getRedirect());
		else
			this->_response.doRedirectResponse(route.getRedirect());
	}
	else if (route.getIndex() != "")
		this->_response = this->readFileToResponse(route.getIndex());
	else
		this->_response = this->readFileToResponse(this->_request.getTargetRoute());
}
void				RequestProcessor::postMethod() {
	
}
void				RequestProcessor::putMethod() {
}
void				RequestProcessor::patchMethod() {
}
void				RequestProcessor::deleteMethod() {
}
void				RequestProcessor::headMethod() {
}
void				RequestProcessor::optionsMethod() {
}
Route				RequestProcessor::resolveRoute(const std::string& routePath) {
	Route route = this->_server->getServerConfig().getRoute(routePath);
	if (route.getRoutePath() != "")
		return (route);
	return (this->_server->getServerConfig().getRoute(routePath.substr(0, routePath.rfind("/") + 1)));
}
void				RequestProcessor::sendResponse(const std::string& responseString) {
	size_t sent = 0;
	while (sent < responseString.size()) {
    	size_t n = ::send(this->_clientFD, responseString.c_str() + sent, responseString.size() - sent, 0);
    	if ((int)n <= 0) {
	        Log::error("Send failed for FD " + stp_itoa(this->_clientFD));
        	return ;
    	}
    	sent += n;
	}
}