#ifndef SERVERCONFIGARRAY_HPP
# define SERVERCONFIGARRAY_HPP

/* 0. Includes*/
#include "webserv.hpp"

/* 1. Class */
class ServerConfigArray {
	private:
	/* M. Member Variables */
		std::vector<ServerConfig>	_serverConfigs;
		int							_error;
	public:
	/* C. Constructors */
		~ServerConfigArray(void);
		ServerConfigArray(void);
		ServerConfigArray(const ServerConfigArray& src);
		ServerConfigArray&	operator=(const ServerConfigArray& src);
		ServerConfigArray(std::string& configurationFilePath);
	/* G. Getters */
	int					getError(void) const;
		ServerConfig&	GetServer(size_t pos);
		size_t			GetSize(void) const;
	private:
	/* PRI0. Private Functions */
	void	deepCopy(const ServerConfigArray& src);
};

#endif