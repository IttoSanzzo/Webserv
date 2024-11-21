#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"

class Server {
	private:
		ServerConfig				_serverConfig;
		int							_socketFd;
		sockaddr_in					_addr;
		std::map<long, std::string>	_requests;
		Server(void);
	public:
	/* C. Constructors */
		~Server(void);
		Server(const Server& src);
		Server&	operator=(const Server& src);
		Server(const ServerConfig& serverConfig);
	/* S. Setters */
		void	setServerConfig(const ServerConfig& serverConfig);
		void	setSocketFd(const int& fd);
	/* G. Getters */
		ServerConfig&	getServerConfig(void);
		int				getSocketFd(void) const;
		sockaddr_in		getAddr(void) const;
	/* PUB0. Core */
	private:
	/* PRI0. Private Functions */
	void	deepCopy(const Server& src);
	void	setAddr(void);
	void	closeFd(void);
	void	closeSocket(const int& socket);
};

#endif