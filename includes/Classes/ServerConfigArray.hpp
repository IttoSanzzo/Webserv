#ifndef SERVERCONFIGARRAY_HPP
# define SERVERCONFIGARRAY_HPP

#include "webserv.hpp"

class ServerConfigArray {
	private:
		std::vector<ServerConfig>	_serverConfigs;
		int							_error;
	public:
		~ServerConfigArray(void);
		ServerConfigArray(void);
		ServerConfigArray(const ServerConfigArray& src);
		ServerConfigArray(std::string& configurationFilePath);
		ServerConfigArray&	operator=(const ServerConfigArray& src);
	int						getError(void) const;
		ServerConfig&		getServer(size_t pos);
		size_t				getSize(void) const;
	private:
	void					deepCopy(const ServerConfigArray& src);
};

#endif