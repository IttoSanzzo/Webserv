#ifndef REQUESTPROCESSOR_HPP
# define REQUESTPROCESSOR_HPP

#include "webserv.hpp"

class RequestProcessor {
	private:
		int				_clientFD;
		Server*			_server;
		HttpRequest		_request;
		HttpResponse	_response;
		RequestProcessor(void);
	public:
		~RequestProcessor(void);
		RequestProcessor(const HttpRequest& request, Server* server, const int& clientFD);
		RequestProcessor(const RequestProcessor& src);
		RequestProcessor&	operator=(const RequestProcessor& src);
		void				setClientFD(const int& clientFD);
		void				setServer(Server* server);
		void				setRequest(const HttpRequest& request);
		bool				process(void);
		HttpResponse		readFileToResponse(const std::string& filePath);
		int					fileExists(std::string filePath);
	private:
		void				deepCopy(const RequestProcessor& src);
		void				doErrorPage(void);
		void				getMethod(void);
		void				postMethod(void);
		void				putMethod(void);
		void				patchMethod(void);
		void				deleteMethod(void);
		void				headMethod(void);
		void				optionsMethod(void);
		Route				resolveRoute(const std::string& routePath);
		void				sendResponse(const std::string& responseString);
};

#endif