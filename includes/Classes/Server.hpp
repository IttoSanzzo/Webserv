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
		~Server(void);
		Server(const Server& src);
		Server(const ServerConfig& serverConfig);
		Server&			operator=(const Server& src);
		void			setServerConfig(const ServerConfig& serverConfig);
		void			setSocketFd(const int& fd);
		ServerConfig&	getServerConfig(void);
		int				getSocketFd(void) const;
		sockaddr_in		getAddr(void) const;
		bool			listenerSetup(void);
		void			closeSocketFd(void);
		void			closeSocketFd(const int& socketFd);
		short			clientSocketCall(const short& clientSocket);
	private:
		void			setAddr(void);
		bool			serveRequest(const HttpRequest& request, const int& clientSocketFd);
		std::string		readRequest(const short& clientSocket);
		void			deepCopy(const Server& src);
};

#endif