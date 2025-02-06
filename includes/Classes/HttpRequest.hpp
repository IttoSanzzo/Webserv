#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include "webserv.hpp"

class HttpRequest {
	private:
		std::string							_originalString;
		t_method							_method;
		std::string							_targetRoute;
		std::string							_host;
		t_protocol							_protocol;
		std::string							_userAgent;
		size_t								_contentLength;
		std::string							_body;
		std::string							_fullCookie;
		std::vector<std::string>			_accept;
		std::vector<std::string>			_acceptEncoding;
		std::map<std::string, std::string>	_others;
		std::map<std::string, std::string>	_cookies;
		std::map<std::string, std::string>	_queryParameters;
	public:
		~HttpRequest(void);
		HttpRequest(void);
		HttpRequest(const HttpRequest& src);
		HttpRequest(const std::string& request);
		HttpRequest&	operator=(const HttpRequest& src);
		
		void			setOriginalString(const std::string& src);
		void			setMethodType(const t_method& method);
		void			setTargetRoute(const std::string& targetRoute);
		void			setHost(const std::string& host);
		void			setProtocol(const t_protocol& protocol);
		void			setUserAgent(const std::string& userAgent);
		void			setContentLength(const size_t& contentLength);
		void			setBody(const std::string& body);
		void			pushAccept(const std::string& accept);
		void			pushAcceptEncoding(const std::string& acceptEncoding);
		std::string		getOriginalString(void) const;
		t_method		getMethodType(void) const;
		std::string		getTargetRoute(void) const;
		std::string		getHost(void) const;
		t_protocol		getProtocol(void) const;
		std::string		getUserAgent(void) const;
		size_t			getContentLength(void) const;
		std::string		getBody(void);
		std::string		getFullCookie(void) const;
		std::string		getAccept(const size_t& pos) const;
		std::string		getAcceptEncoding(const size_t& pos) const;
		std::string		getOther(const std::string& name);
		std::string		getCookie(const std::string& name);
		std::string		getQueryParameter(const std::string& name);
		std::map<std::string, std::string>&	getQueryParameters(void);
		std::map<std::string, std::string>&	getOther(void);
		void			setHeaderPart(const std::string& header);
		void			setHeaderSwitch(const std::string& name, const std::string& value);
		void			doQueryParameters(const std::string& fullRoute);
		std::string		toString(void);
	private:
		void			deepCopy(const HttpRequest& src);
};

#endif