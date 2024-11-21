#ifndef WS_DATASTRUCTURES_HPP
# define WS_DATASTRUCTURES_HPP

#include "webserv.hpp"

typedef struct	s_listen {
	// 0. Variables
	in_addr_t	host;
	uint16_t	port;
	// 1. Overloads
	bool	operator==(const s_listen& other) const {return (host == other.host && port == other.port);}
	bool	operator!=(const s_listen& other) const {return (!(*this == other));}
	std::string	toString(void) {
		std::ostringstream	ossHost;
		std::ostringstream	ossPort;
		ossHost << host;
		ossPort << port;
		return (ossHost.str() + ":" + ossPort.str());
	}
}	t_listen;

typedef pollfd pollfd;
typedef sockaddr_in sockaddr_in;

#endif