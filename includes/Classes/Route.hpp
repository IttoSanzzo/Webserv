#ifndef ROUTE_HPP
# define ROUTE_HPP

#include "webserv.hpp"

class	Route {
	private:
		std::string							_routePath;
		std::string							_index;
		std::string							_redirect;
		std::string							_savePath;
		bool								_autoindex;
		bool								_cgi;
		std::map<std::string, bool>			_methods;
	public:
		~Route(void);
		Route(void);
		Route(const Route& src);
		Route(const JsonNode& routeJson);
		Route&		operator=(const Route& src);
		void		setRoutePath(const std::string& routePath);
		void		setIndex(const std::string& index);
		void		setSavePath(const std::string& savePath);
		void		setAutoindex(const bool& value);
		void		setCgi(const bool& value);
		void		setMethods(const std::string& method, const bool& value);
		std::string	getRoutePath(void) const;
		std::string	getIndex(void) const;
		std::string	getRedirect(void) const;
		std::string	getSavePath(void) const;
		bool		getAutoindex(void) const;
		bool		getCgi(void) const;
		bool		getMethod(const t_method& method);
		bool		getMethod(const std::string& method);
	private:
		void		deepCopy(const Route& src);
		void		setRoutePath(const JsonNode& routeJson);
		void		setIndex(const JsonNode& routeJson);
		void		setRedirect(const JsonNode& routeJson);
		void		setSavePath(const JsonNode& routeJson);
		void		setAutoindex(const JsonNode& routeJson);
		void		setCgi(const JsonNode& routeJson);
		void		setMethods(const JsonNode& routeJson);
	public:
		class	ErrorException : public std::exception {
			private:
				std::string	_message;
			public:
				virtual ~ErrorException() throw() {}
				ErrorException(std::string message) throw() {
					this->_message = "Route: " + message;
				}
				virtual const char*	what() const throw() {
					return (_message.c_str());
				}
		};
};

#endif