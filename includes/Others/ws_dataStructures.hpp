#ifndef WS_DATASTRUCTURES_HPP
# define WS_DATASTRUCTURES_HPP

#include "webserv.hpp"
std::string	ws_inet_ntoa(in_addr_t addr);

typedef struct	s_listen {
	// 0. Variables
	in_addr_t	host;
	uint16_t	port;
	// 1. Overloads
	bool	operator==(const s_listen& other) const {return (host == other.host && port == other.port);}
	bool	operator!=(const s_listen& other) const {return (!(*this == other));}
	std::string	toString(void) {
		std::ostringstream	ossPort;
		ossPort << port;
		return (std::string(ws_inet_ntoa(ntohl(host))) + ":" + ossPort.str());
	}
}	t_listen;
typedef enum e_protocol {
	no_protocol,
	http1dot1
}	t_protocol;
typedef enum e_contentType {
	no_type,
	textHtml,
	textCss,
	applicationJavascript
}	t_contentType;
typedef enum e_method {
	no_method,
	GET,
	POST,
	PUT,
	PATCH,
	DELETE,
	HEAD,
	OPTIONS
}	t_method;

typedef pollfd pollfd;
typedef sockaddr_in sockaddr_in;

#endif