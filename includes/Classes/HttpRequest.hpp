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
		std::vector<std::string>			_accept;
		std::vector<std::string>			_acceptEncoding;
		std::map<std::string, std::string>	_others;
	public:
		~HttpRequest(void);
		HttpRequest(void);
		HttpRequest(const HttpRequest& src);
		HttpRequest(const std::string& request);
		HttpRequest			operator=(const HttpRequest& src);
		void				setSwitch(const std::string& name, const std::string& value);
		void				setOriginalString(const std::string& src);
		void				setMethod(const t_method& method);
		void				setTargetRoute(const std::string& targetRoute);
		void				setHost(const std::string& host);
		void				setProtocol(const t_protocol& protocol);
		void				setUserAgent(const std::string& userAgent);
		void				pushAccept(const std::string& accept);
		void				pushAcceptEncoding(const std::string& acceptEncoding);
		std::string			getOriginalString(void) const;
		t_method			getMethod(void) const;
		std::string			getTargetRoute(void) const;
		std::string			getHost(void) const;
		t_protocol			getProtocol(void) const;
		std::string			getUserAgent(void) const;
		std::string			getAccept(const size_t& pos) const;
		std::string			getAcceptEncoding(const size_t& pos) const;
		std::string			getOther(const std::string& name);
		std::map<std::string, std::string>&	getOther(void);
		static t_method		methodFromString(const std::string& method);
		static std::string	methodToString(const t_method& method);
		static t_protocol	protocolFromString(const std::string& protocol);
		static std::string	protocolToString(const t_protocol& protocol);
		std::string			toString(void);
	private:
		void				deepCopy(const HttpRequest& src);
};

#endif