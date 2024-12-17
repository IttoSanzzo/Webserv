#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include "webserv.hpp"

class Cluster {
	private:
		static int					_signalValue;
		ServerConfigArray			_serverConfigs;
		std::vector<Server>			_servers;
		size_t						_requests;
		int							_error;
		std::vector<pollfd>			_pollFds;
		std::map<int, t_clientConn>	_clientConnMap;
	public:
		~Cluster(void);
		Cluster(const Cluster& src);
		Cluster(std::string& configurationFilePath);
		Cluster&			operator=(const Cluster& src);
		ServerConfigArray&	getServerConfigArray(void);
		ServerConfig&		getServerConfig(const size_t& pos);
		size_t				getServerConfigSize(void) const;
		int					getError(void) const;
		std::vector<pollfd>	getPollFds(void) const;
		void				runCluster(void);
	private:
		void				runPoll(void);
		void				deepCopy(const Cluster& src);
		static void			signalHandler(int signal);
		void				setPollFds(std::vector<pollfd> pollFds);
		bool				serversStart(void);
		bool				serversListenersSetup(void);
		void				pollFdSetup(void);
		void				handleClient(const size_t& pollIndex);
		void				acceptNewClient(const size_t& pollIndex);
		void				handleClientRequest(const size_t& pollIndex);
		void				checkTimeouts(const time_t& now);
		void				closePollFd(const size_t& pollIndex);
	public:
		class	ErrorException : public std::exception {
			private:
				std::string	_message;
			public:
				virtual ~ErrorException() throw() {}
				ErrorException(std::string message) throw() {
					this->_message = "Cluster Error :\033[0m " + message;
				}
				virtual const char*	what() const throw() {
					return (_message.c_str());
				}
		};
};

#endif