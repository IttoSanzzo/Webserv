#include "webserv.hpp"

t_protocol	protocolFromString(const std::string& protocol) {
	if (protocol == "HTTP/1.1")
		return (http1dot1);
	else
		return (no_protocol);
}
std::string	protocolToString(const t_protocol& protocol) {
	switch (protocol) {
		case (http1dot1):
			return ("HTTP/1.1");
		break;
		default:
			return ("");
		break;
	}
}