#ifndef SERVERARRAY_HPP
# define SERVERARRAY_HPP

/* 0. Includes*/
#include "webserv.hpp"

/* 1. Class */
class ServerArray {
	private:
	/* M. Member Variables */
		ServerConfig*	servers;
		size_t			size;
	public:
	/* C. Contrusctors */
		~ServerArray(void);
		ServerArray(void);
		ServerArray(std::string& configurationFilePath);
	/* G. Getters */
		ServerConfig*	GetServer(size_t pos) const;
		size_t			GetSize(void) const;
};

#endif