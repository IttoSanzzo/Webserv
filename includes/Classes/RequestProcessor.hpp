#ifndef REQUESTPROCESSOR_HPP
# define REQUESTPROCESSOR_HPP

#include "webserv.hpp"

class RequestProcessor {
	private:
		Server*			_server;
		HttpRequest		_request;
		HttpResponse	_response;
		RequestProcessor(void);
	public:
		~RequestProcessor(void);
		RequestProcessor(const HttpRequest& request, Server* server);
		RequestProcessor(const RequestProcessor& src);
		RequestProcessor&	operator=(const RequestProcessor& src);
		void				setServer(Server* server);
		void				setRequest(const HttpRequest& request);
		void				process(const int& socketFd);
		HttpResponse		readFileToResponse(const std::string& filePath);
		int					fileExists(std::string filePath);
	private:
		void				deepCopy(const RequestProcessor& src);
		void				doErrorPage(void);
		void				getMethod(const int& socketFd);
		void				postMethod(const int& socketFd);
		void				putMethod(const int& socketFd);
		void				patchMethod(const int& socketFd);
		void				deleteMethod(const int& socketFd);
		void				headMethod(const int& socketFd);
		void				optionsMethod(const int& socketFd);
		Route				getRoute(const std::string& route);
		void				sendResponse(const int& socketFd, const std::string& responseString);
};

#endif