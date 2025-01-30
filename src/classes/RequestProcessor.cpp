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
	
	// Log::debug(this->_request.getOriginalString());
	// Log::debug(this->_request.getBody());
	
	Route route = this->resolveRoute(this->_request.getTargetRoute());
	if (route.getRoutePath() == "")
		this->_response.setCode(404);
	else if (route.getMethod(this->_request.getMethodType()) == false)
		this->_response.setCode(405);
	else
		switch (this->_request.getMethodType()) {
			case (GET):
				this->getMethod(route);
			break;
			case (POST):
				this->postMethod(route);
			break;
			case (PUT):
				this->putMethod(route);
			break;
			case (PATCH):
				this->patchMethod(route);
			break;
			case (DELETE):
				this->deleteMethod(route);
			break;
			case (HEAD):
				this->headMethod(route);
			break;
			case (OPTIONS):
				this->optionsMethod(route);
			break;
			default:
			break;
		}
	if (!(this->_response.getCode() >= 200 && this->_response.getCode() < 300) && this->_response.getCode() != 301)
		this->doErrorPage();
	if (this->_request.getOther("Connection") == "keep-alive" && this->_response.getCode() != 301)
		this->_response.setKeepAlive(true);
	if (this->send(this->_response.getFullHeader()) == false
		|| this->send(this->_response.getContent()) == false)
		return (false);
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
void				RequestProcessor::getMethod(const Route& route) { 
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
void				RequestProcessor::postMethod(const Route& route) {
	if (this->_request.getBody() == "") {
		this->_response.setCode(400);
		return ;
	}
	std::vector<std::string> bodyParts = stp_split(this->_request.getBody(), this->_request.getBody().substr(0, this->_request.getBody().find('\n') - 1));
	for (size_t i = 1; i < bodyParts.size() - 1; ++i) {
		bodyParts[i] = bodyParts[i].substr(2, bodyParts[i].size() - 4);
		int	startPoint = bodyParts[i].find("\r\n\r\n") + 4;
		int	filenameStart = bodyParts[i].find("filename=\"") + 10;
		std::string content(bodyParts[i].substr(startPoint, bodyParts[i].rfind(bodyParts[i].substr(0, bodyParts[i].find('\n') - 1)) - 2 - startPoint));
		std::string filename(bodyParts[i].substr(filenameStart, bodyParts[i].find("\"", filenameStart) - filenameStart));
		this->createFile(route.getRoutePath(), filename, content);
	}
	this->_response.setCode(201);
	this->_response.setType(textPlain);
	this->_response.setContent(httpStatusCodeToString(this->_response.getCode()));
}
void				RequestProcessor::putMethod(const Route& route) {
	(void)route;
	this->_response.setCode(501);
	this->_response.setType(textPlain);
	this->_response.setContent(httpStatusCodeToString(this->_response.getCode()));
}
void				RequestProcessor::patchMethod(const Route& route) {
	(void)route;
	this->_response.setCode(501);
	this->_response.setType(textPlain);
	this->_response.setContent(httpStatusCodeToString(this->_response.getCode()));
}
void				RequestProcessor::deleteMethod(const Route& route) {
	(void)route;
	this->_response.setCode(501);
	this->_response.setType(textPlain);
	this->_response.setContent(httpStatusCodeToString(this->_response.getCode()));
}
void				RequestProcessor::headMethod(const Route& route) {
	(void)route;
	this->_response.setCode(501);
	this->_response.setType(textPlain);
	this->_response.setContent(httpStatusCodeToString(this->_response.getCode()));
}
void				RequestProcessor::optionsMethod(const Route& route) {
	(void)route;
	this->_response.setCode(501);
	this->_response.setType(textPlain);
	this->_response.setContent(httpStatusCodeToString(this->_response.getCode()));
}
Route				RequestProcessor::resolveRoute(const std::string& routePath) {
	Route route = this->_server->getServerConfig().getRoute(routePath);
	if (route.getRoutePath() != "")
		return (route);
	return (this->_server->getServerConfig().getRoute(routePath.substr(0, routePath.rfind("/") + 1)));
}
bool				RequestProcessor::send(const std::string& message) {
	char clientTest[1];
	size_t n;
	size_t sent = 0;
	while (sent < message.size()) {
		if (recv(this->_clientFD, clientTest, 1, MSG_DONTWAIT) == 0) {
			Log::error("The client closed connection while sending.");
			return (false);
		}
		Log::debugNoEndl("Sending... ");
    	n = ::send(this->_clientFD, message.c_str() + sent, message.size() - sent,  MSG_NOSIGNAL);
		Log::debugNoTimestamp("Sent " + stp_itoa(n) + std::string(" bytes"));
    	if ((int)n <= 0) {
	        Log::error("Send failed for FD " + stp_itoa(this->_clientFD));
        	return (false);
    	}
    	sent += n;
	}
	return (true);
}
int					RequestProcessor::createFile(const std::string& path, const std::string& filename, const std::string& content) {
	std::ofstream file(std::string(std::string("./public") + path + std::string("/") + filename).c_str(), std::ios::out | std::ios::binary);
    if (!file) {
        Log::error("Error while creating file.");
		return (-1);
	}
	file.write(content.c_str(), content.size());
	file.close();
	return (0);
}