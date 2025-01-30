#ifndef ROUTE_HPP
# define ROUTE_HPP

#include "webserv.hpp"

class	Route {
	private:
		std::string							_routePath;
		std::string							_index;
		std::string							_root;
		std::string							_redirect;
		bool								_autoindex;
		std::map<std::string, std::string>	_cgiRelations;
		std::map<std::string, bool>			_methods;
	public:
		~Route(void);
		Route(void);
		Route(const Route& src);
		Route(const JsonNode& routeJson);
		Route&		operator=(const Route& src);
		void		setRoutePath(const std::string& routePath);
		void		setIndex(const std::string& index);
		void		setRoot(const std::string& root);
		void		setAutoindex(const bool& value);
		void		setMethods(const std::string& method, const bool& value);
		void		setCgiRelation(const std::string& extension, const std::string& path);
		std::string	getRoutePath(void) const;
		std::string	getIndex(void) const;
		std::string	getRoot(void) const;
		std::string	getRedirect(void) const;
		bool		getAutoindex(void) const;
		bool		getMethod(const t_method& method);
		bool		getMethod(const std::string& method);
		std::string	getCgiPath(const std::string& extension);
	private:
		void		deepCopy(const Route& src);
		void		setRoutePath(const JsonNode& routeJson);
		void		setIndex(const JsonNode& routeJson);
		void		setRoot(const JsonNode& routeJson);
		void		setRedirect(const JsonNode& routeJson);
		void		setAutoindex(const JsonNode& routeJson);
		void		setMethods(const JsonNode& routeJson);
		void		setCgiRelation(const JsonNode& routeJson);
		void		parseCgiChild(const JsonNode& errorChild);
		std::string	parseCgiElement(const JsonNode& errorChild, const std::string& element);
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