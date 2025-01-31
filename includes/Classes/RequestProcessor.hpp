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
		void				getMethod(const Route& route);
		void				postMethod(const Route& route);
		void				putMethod(const Route& route);
		void				patchMethod(const Route& route);
		void				deleteMethod(const Route& route);
		void				headMethod(const Route& route);
		void				optionsMethod(const Route& route);
		Route				resolveRoute(const std::string& routePath);
		void				autoIndexingResponse(const std::string& targetRoute);		
		std::string			autoIndexingHTML(DIR* dir);
		bool				send(const std::string& message);
		int					createFile(const std::string& path, const std::string& filename, const std::string& content);
};

#endif