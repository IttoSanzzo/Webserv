#ifndef SERVERCONFIGARRAY_HPP
# define SERVERCONFIGARRAY_HPP

/* 0. Includes*/
#include "webserv.hpp"

/* 1. Class */
class ServerConfigArray {
	private:
	/* M. Member Variables */
		ServerConfig*	serverConfigs;
		size_t			size;
	public:
	/* C. Contrusctors */
		~ServerConfigArray(void);
		ServerConfigArray(void);
		ServerConfigArray(std::string& configurationFilePath);
	/* G. Getters */
		ServerConfig*	GetServer(size_t pos) const;
		size_t			GetSize(void) const;
};

#endif