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
typedef struct	s_clientConn {
	// 0. Variables
	int		fd;
	Server*	server;
	time_t	last_activity;
	// 1. Overloads
	bool	timeout_safe(const time_t& now) {
		if (now - last_activity > CLIENTTIMEOUT)
			return (false);
		return (true);
	}
}	t_clientConn;
typedef enum e_protocol {
	no_protocol,
	http1dot1
}	t_protocol;
typedef enum e_contentType {
	anyType,
	textHtml,
	textCss,
	applicationJavascript,
	imagePng,
	imageJpeg,
	videoMp4,
	appXWwwFormUrlencoded,
	multipartFormData
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