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
	
}
std::string			RequestProcessor::readHtml(const std::string& filePath)
{
	HttpResponse		htmlResponse;
	std::ofstream		file;
	std::stringstream	buffer;

	htmlResponse.setType(textHtml);
	if (this->fileExists(filePath)) {
		file.open(filePath.c_str(), std::ifstream::in);
		if (file.is_open() == false) {
			htmlResponse.setCode(40404);
			htmlResponse.setContent(ERRORPAGEERROR);
		} else {
			buffer << file.rdbuf();
			file.close();
			htmlResponse.setCode(200);
			htmlResponse.setContent(buffer.str());
		}
	}
	else {
		htmlResponse.setCode(40404);
		htmlResponse.setContent(ERRORPAGEERROR);
	}
	this->_response.push_back(htmlResponse);
}
int					RequestProcessor::fileExists(std::string filePath) //deprecated, replaced by ::pathIsFile()
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