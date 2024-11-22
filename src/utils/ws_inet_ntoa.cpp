#include "webserv.hpp"

std::string	ws_inet_ntoa(in_addr_t addr) {
	std::ostringstream	ip;
	ip << ((addr >> 24) & 0xFF) << '.'
		<< ((addr >> 16) & 0xFF) << '.'
		<< ((addr >> 8) & 0xFF) << '.'
		<< (addr & 0xFF);
	return (ip.str());
}