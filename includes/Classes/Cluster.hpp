#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include "webserv.hpp"

class Cluster {
	private:
		static int			_signalValue;
		ServerConfigArray	_serverConfigs;
		std::vector<Server>	_servers;
		size_t				_requests;
		int					_error;
		pollfd*				_pollFd;
	public:
		~Cluster(void);
		Cluster(const Cluster& src);
		Cluster(std::string& configurationFilePath);
		Cluster&			operator=(const Cluster& src);
		ServerConfigArray&	getServerConfigArray(void);
		ServerConfig&		getServerConfig(const size_t& pos);
		size_t				getServerConfigSize(void) const;
		int					getError(void) const;
		pollfd*				getPollFd(void) const;
		void				runCluster(void);
	private:
		void				runPoll(void);
		void				deepCopy(const Cluster& src);
		static void			signalHandler(int signal);
		void				setPollFd(pollfd* pollFd);
		void				freePollFd(void);
		bool				serversStart(void);
		bool				serversListenersSetup(void);
		void				pollFdSetup(void);
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