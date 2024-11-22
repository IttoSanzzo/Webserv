#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include "webserv.hpp"

class HttpResponse {
	private:
		short			_code;
		t_contentType	_contentType;
		std::string		_content;
	public:
		~HttpResponse(void);
		HttpResponse(void);
		HttpResponse(const HttpResponse& src);
		HttpResponse		operator=(const HttpResponse& src);
		void				setCode(const short& code);
		void				setType(const t_contentType& type);
		void				setContent(const std::string& content);
		short				getCode(void) const;
		std::string			getCodeDescription(void) const;
		std::string			getCodeDescription(const short& code) const;
		t_contentType		getType(void) const;
		size_t				getHeaderLength(void) const;
		size_t				getContentLength(void) const;
		std::string			getContent(void) const;
		size_t				getSize(void) const;
		std::string			getHeader(void) const;
		std::string			toString(void) const;
		static std::string	contentTypeString(const t_contentType& type);
	private:
		void			deepCopy(const HttpResponse& src);
};

#endif