#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "webserv.hpp"

class	Location {
	private:
		std::string							_page;
		std::string							_index;
		std::string							_root;
		std::string							_returner;
		bool								_autoindex;
		std::map<std::string, std::string>	_cgiRelations;
		std::map<std::string, bool>			_methods;
	public:
		~Location(void);
		Location(void);
		Location(const Location& src);
		Location(const JsonNode& locationJson);
		Location&	operator=(const Location& src);
		void		setPage(const std::string& page);
		void		setIndex(const std::string& index);
		void		setRoot(const std::string& root);
		void		setReturner(const std::string& target);
		void		setAutoindex(const bool& value);
		void		setMethods(const std::string& method, const bool& value);
		void		setCgiRelation(const std::string& extension, const std::string& path);
		std::string	getPage(void) const;
		std::string	getIndex(void) const;
		std::string	getRoot(void) const;
		std::string	getReturner(void) const;
		bool		getAutoindex(void) const;
		bool		getMethod(const std::string& method);
		std::string	getCgiPath(const std::string& extension);
	private:
		void		deepCopy(const Location& src);
		void		setPage(const JsonNode& locationJson);
		void		setIndex(const JsonNode& locationJson);
		void		setRoot(const JsonNode& locationJson);
		void		setReturner(const JsonNode& locationJson);
		void		setAutoindex(const JsonNode& locationJson);
		void		setMethods(const JsonNode& locationJson);
		void		setCgiRelation(const JsonNode& locationJson);
		void		parseCgiChild(const JsonNode& errorChild);
		std::string	parseCgiElement(const JsonNode& errorChild, const std::string& element);
	public:
		class	ErrorException : public std::exception {
			private:
				std::string	_message;
			public:
				virtual ~ErrorException() throw() {}
				ErrorException(std::string message) throw() {
					this->_message = "Location: " + message;
				}
				virtual const char*	what() const throw() {
					return (_message.c_str());
				}
		};
};

#endif