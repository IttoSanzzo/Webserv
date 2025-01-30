#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include "webserv.hpp"

class HttpResponse {
	private:
		short			_code;
		t_contentType	_contentType;
		std::string		_content;
		bool			_keep_alive;
		bool			_redirect;
	public:
		~HttpResponse(void);
		HttpResponse(void);
		HttpResponse(const HttpResponse& src);
		HttpResponse&		operator=(const HttpResponse& src);
		void				setCode(const short& code);
		void				setType(const t_contentType& type);
		void				setContent(const std::string& content);
		void				setKeepAlive(const bool& keepAlive);
		void				setRedirect(const bool& redirect);
		short				getCode(void) const;
		t_contentType		getType(void) const;
		size_t				getHeaderLength(void) const;
		size_t				getContentLength(void) const;
		std::string			getContent(void) const;
		bool				getKeepAlive(void) const;
		bool				getRedirect(void) const;
		size_t				getSize(void) const;
		std::string			getHeader(void) const;
		std::string			getFullHeader(void) const;
		std::string			toString(void) const;
		void				doRedirectResponse(const std::string& target);
	private:
		void				deepCopy(const HttpResponse& src);
};

#endif