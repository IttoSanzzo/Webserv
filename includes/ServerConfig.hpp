#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

#include "webserv.hpp"

class	Location;

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
		// std::vector<Location>		_locations; // TODO LOCATIONS 1
		struct sockaddr_in				_server_address;
	public:
	/* C. Constructors */
		~ServerConfig();
		ServerConfig();
		ServerConfig(const ServerConfig& src);
		ServerConfig&	operator=(const ServerConfig& src);
	/* S. Setters */
		void	setPort(std::string port);
		void	setHost(std::string host);
		void	setServerName(std::string server_name);
		void	setRoot(std::string root);
		void	setIndex(std::string index);
		void	setAutoindex(bool autoindex);
		void	setFd(int fd);
		void	setClientMaxBodySize(int body_size);
	/* G. Getters */
		u_int16_t							getPort(void) const;
		in_addr_t							getHost(void) const;
		std::string						getServerName(void) const;
		std::string						getRoot(void) const;
		std::string						getIndex(void) const;
		bool								getAutoindex(void) const;
		int								getFd(void) const;
		size_t							getClientMaxBodySize(void) const;
	/* 0. Core */
		void		initErrorPages(void);
		bool		isValidHost(std::string host) const;
		// bool		isValidErrorPages(void); //TODO ServerConfig::isValidErrorPages
		// int		isValidLocation(Location &location) const; //TODO ServerConfig::isValidLocation
		// bool		checkLocations(void) const; //TODO ServerConfig::checkLocation
		// void		setupServer(void); //TODO ServerConfig::setupServer
	/* E. Exception */
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
