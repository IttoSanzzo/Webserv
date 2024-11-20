#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

#include "webserv.hpp"

class	ServerConfig {
	private:
		uint16_t						_port;
		in_addr_t						_host;
		std::string						_server_name;
		std::string						_root;
		std::string						_index;
		bool							_autoindex;
		int								_listen_fd;
		unsigned long					_client_max_body_size;
		std::map<short, std::string>	_error_pages;
		std::map<std::string, Location>	_locations;
		struct sockaddr_in				_server_address;
	/* C. Constructors */
	public:
		~ServerConfig();
		ServerConfig();
		ServerConfig(const ServerConfig& src);
		ServerConfig&	operator=(const ServerConfig& src);
		ServerConfig(const JsonNode& configJson);
	/* S. Setters */
		void	setPort(const std::string& port);
		void	setHost(std::string host);
		void	setServerName(const std::string& server_name);
		void	setRoot(const std::string& root);
		void	setIndex(const std::string& index);
		void	setAutoindex(bool autoindex);
		void	setFd(int fd);
		void	setClientMaxBodySize(int body_size);
	/* G. Getters */
		u_int16_t	getPort(void) const;
		in_addr_t	getHost(void) const;
		std::string	getServerName(void) const;
		std::string	getRoot(void) const;
		std::string	getIndex(void) const;
		bool		getAutoindex(void) const;
		size_t		getClientMaxBodySize(void) const;
		std::string	getErrorPage(const short& pos);
		Location	getLocation(const std::string& page);
		int			getFd(void) const;
	/* 0. Core */
	public:
		void		initErrorPages(void);
		bool		isValidHost(std::string host) const;
		// bool		isValidErrorPages(void); //TODO ServerConfig::isValidErrorPages
		// int		isValidLocation(Location &location) const; //TODO ServerConfig::isValidLocation
		// bool		checkLocations(void) const; //TODO ServerConfig::checkLocation
		// void		setupServer(void); //TODO ServerConfig::setupServer
	/* 1. JsonParsing*/
	private:
		void		setPort(const JsonNode& configJson);
		void		setHost(const JsonNode& configJson);
		void		setServerName(const JsonNode& configJson);
		void		setRoot(const JsonNode& configJson);
		void		setIndex(const JsonNode& configJson);
		void		setAutoindex(const JsonNode& configJson);
		void		setClientMaxBodySize(const JsonNode& configJson);
		void		setErrorPages(const JsonNode& configJson);
		void		setLocations(const JsonNode& configJson);
		void		parseErrorChild(const JsonNode& errorChild);
		std::string	parseErrorElement(const JsonNode& errorChild, const std::string& element);
	/* PRIU. Utils */
		void	deepCopy(const ServerConfig& src);
	/* E. Exception */
	public:
		class	ErrorException : public std::exception {
			private:
				std::string	_message;
			public:
				virtual ~ErrorException() throw() {}
				ErrorException(std::string message) throw() {
					this->_message = ERR_EXCE + message;
				}
				virtual const char*	what() const throw() {
					return (_message.c_str());
				}
		};
};

#endif
