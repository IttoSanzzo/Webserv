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
		unsigned long					_cli_max_body_size;
		std::map<short, std::string>	_error_pages;
		// std::vector<Location>		_locations; // TODO LOCATIONS 1
		struct sockaddr_in				_server_address;
	public:
	/* Commons */
		~ServerConfig();
		ServerConfig();
		ServerConfig(const ServerConfig& src);
		ServerConfig&	operator=(const ServerConfig& src);
	/* Setters */
		void	setPort(std::string port);
		void	setHost(std::string host);
		void	setServerName(std::string server_name);
		void	setRoot(std::string root);
		void	setIndex(std::string index);
		void	setAutoindex(std::string autoindex);
		void	setFd(int fd);
		void	setClientMaxBodySize(std::string body_size);
		void	setErrorPages(std::vector<std::string> &parameter);
		// void	setLocations(std::string nameLocation, std::vector<std::string> parameter); //TODO ServerConfig::setLocations
	/* Getters */
		const u_int16_t&						getPort(void) const;
		const in_addr_t&						getHost(void) const;
		const std::string&						getServerName(void) const;
		const std::string&						getRoot(void) const;
		const std::string&						getIndex(void) const;
		const bool&								getAutoindex(void) const;
		const int&								getFd(void) const;
		const size_t&							getClientMaxBodySize(void) const;
		// const std::map<short, std::string>		&getErrorPages(void) const; //TODO ServerConfig::getErrorPages
		// const std::string						&getPathErrorPage(short key) const; //TODO ServerConfig::getPathErrorPage
		// const std::vector<Location>				&getLocations(void) const; //TODO ServerConfig::getLocations
		// const std::vector<Location>::iterator	getLocationKey(std::string key) const; //TODO ServerConfig::getLocationKey
	/* Others */
		void		initErrorPages(void);
		bool		isValidHost(std::string host) const;
		static void	checkToken(std::string &parameter);
		// bool		isValidErrorPages(void); //TODO ServerConfig::isValidErrorPages
		// int			isValidLocation(Location &location) const; //TODO ServerConfig::isValidLocation
		// bool		checkLocations(void) const; //TODO ServerConfig::checkLocation
		// void		setupServer(void); //TODO ServerConfig::setupServer
	/* Exception */
		class	ErrorException : public std::exception {
			private:
				std::string _message;
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
