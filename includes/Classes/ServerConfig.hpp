#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

#include "webserv.hpp"

class	ServerConfig {
	private:
		t_listen						_listen;
		std::string						_server_name;
		std::string						_root;
		std::string						_index;
		bool							_autoindex;
		unsigned long					_client_max_body_size;
		std::map<short, std::string>	_error_pages;
		std::map<std::string, Location>	_locations;
	public:
		~ServerConfig(void);
		ServerConfig(void);
		ServerConfig(const ServerConfig& src);
		ServerConfig(const JsonNode& configJson);
		ServerConfig&	operator=(const ServerConfig& src);
		void			setListen(const t_listen& listen);
		void			setPort(const std::string& port);
		void			setHost(std::string host);
		void			setServerName(const std::string& server_name);
		void			setRoot(const std::string& root);
		void			setIndex(const std::string& index);
		void			setAutoindex(bool autoindex);
		void			setClientMaxBodySize(int body_size);
		t_listen		getListen(void) const;
		u_int16_t		getPort(void) const;
		in_addr_t		getHost(void) const;
		std::string		getServerName(void) const;
		std::string		getRoot(void) const;
		std::string		getIndex(void) const;
		bool			getAutoindex(void) const;
		size_t			getClientMaxBodySize(void) const;
		std::string		getErrorPage(const short& pos);
		Location		getLocation(const std::string& page);
		std::map<short, std::string>&		getErrorPagesMap(void);
		std::map<std::string, Location>&	getLocationsMap(void);
		std::string		toString(void);
		void			initErrorPages(void);
		bool			isValidHost(std::string host) const;
	private:
		void			deepCopy(const ServerConfig& src);
		void			setPort(const JsonNode& configJson);
		void			setHost(const JsonNode& configJson);
		void			setServerName(const JsonNode& configJson);
		void			setRoot(const JsonNode& configJson);
		void			setIndex(const JsonNode& configJson);
		void			setAutoindex(const JsonNode& configJson);
		void			setClientMaxBodySize(const JsonNode& configJson);
		void			setErrorPages(const JsonNode& configJson);
		void			setLocations(const JsonNode& configJson);
		void			parseErrorChild(const JsonNode& errorChild);
		std::string		parseErrorElement(const JsonNode& errorChild, const std::string& element);
	public:
		class	ErrorException : public std::exception {
			private:
				std::string	_message;
			public:
				virtual ~ErrorException() throw() {}
				ErrorException(std::string message) throw() {
					this->_message = "ServerConfig Error :\033[0m " + message;
				}
				virtual const char*	what() const throw() {
					return (_message.c_str());
				}
		};
};

#endif
