#include "webserv.hpp"

RequestProcessor::~RequestProcessor(void) {}
RequestProcessor::RequestProcessor(void) {}
RequestProcessor::RequestProcessor(const HttpRequest& request, Server* server) {
	this->_server = server;
	this->_request = request;
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
	this->_response.push_back(HttpResponse());
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
	if (this->_response[0].getCode() != 200)
		this->doErrorPage();
	for (size_t i = 0; i < this->_response.size(); ++i) {
		std::string	completeResponse(this->_response[i].toString());
		::send(socketFd, completeResponse.c_str(), completeResponse.size(), 0);
	}
	::close(socketFd);
	return (0);
}
HttpResponse		RequestProcessor::readFile(const std::string& filePath) {
	HttpResponse		htmlResponse;
	std::ofstream		file;
	std::stringstream	buffer;
	std::string			finalFilePath("./" + std::string("public/") + filePath);
	if (this->fileExists(finalFilePath)) {
		file.open(finalFilePath.c_str(), std::ifstream::in);
		if (file.is_open() == false)
			htmlResponse.setCode(403);
		else {
			htmlResponse.setType(textHtml);
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
int					RequestProcessor::fileExists(std::string filePath)
{
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
	Log::log("Doing Error Page " + stp_itoa(this->_response[0].getCode()));
	this->_response[0].setType(textHtml);
	HttpResponse	errorResponse;
	std::string		errorPagePath(this->_server->getServerConfig().getErrorPage(this->_response[0].getCode()));
	if (errorPagePath != "") {
		errorResponse = this->readFile(errorPagePath);
		if (errorResponse.getCode() == 200) {
			errorResponse.setCode(this->_response[0].getCode());
			this->_response[0] = errorResponse;
			return ;
		}
		errorResponse.setCode(this->_response[0].getCode());
		errorResponse.setContent(ERRORPAGEERROR);
		this->_response[0] = errorResponse;
		return ;
	}
	errorResponse.setCode(this->_response[0].getCode());
	errorResponse.setContent(ERRORPAGE);
	this->_response[0] = errorResponse;
}
void				RequestProcessor::getMethod(const int& socketFd) {
	Location	location(this->getLocation(this->_request.getTargetRoute()));
	if (location.getMethod("GET") == false) {
		this->_response[0].setCode(403);
		return ;
	}
	this->_response[0] = this->readFile(location.getIndex());
	if (this->_response[0].getCode() != 200)
		return ;
	if (location.getIndex().substr(location.getIndex().size() - 5) == ".html")
		this->getHtmlComplements(this->_response[0].getContent());
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
Location			RequestProcessor::getLocation(const std::string& route) {
	return (this->_server->getServerConfig().getLocation(route));
}
void				getHtmlComplements(const std::string& htmlContent) {
	size_t extra = htmlContent.find("rel = \"stylesheet\"");
	if (a)
	Log::debug("YAHOO");
}