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
	public:
	/* C. Constructors */
		~Cluster(void);
		Cluster(const Cluster& src);
		Cluster&	operator=(const Cluster& src);
		Cluster(std::string& configurationFilePath);
	/* G. Getters */
		int					getError(void) const;
		ServerConfigArray&	getServerConfigArray(void);
		ServerConfig&		getServerConfig(const size_t& pos);
		size_t				getServerConfigSize(void) const;
	/* PUB0. Core */
		void	runCluster(void);
	private:
	/* PRI0. Private Functions */
		void		deepCopy(const Cluster& src);
		static void	signalHandler(int signal);
	public:
	/* E. Exception */
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