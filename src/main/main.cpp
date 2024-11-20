#include "webserv.hpp"


int	ws_testing(ServerConfig* conf) {
	/* View */
	std::cout << "Port.......: " << conf->getPort() << std::endl;
	std::cout << "Host.......: " << conf->getHost() << std::endl;
	std::cout << "Name.......: " << conf->getServerName() << std::endl;
	std::cout << "Root.......: " << conf->getRoot() << std::endl;
	std::cout << "Index......: " << conf->getIndex() << std::endl;
	std::cout << "Autoindex..: " << conf->getAutoindex() << std::endl;
	std::cout << "FD.........: " << conf->getFd() << std::endl;
	std::cout << "CliMax.....: " << conf->getClientMaxBodySize() << std::endl;
	std::cout << std::endl;
	std::cout << "404........: " << conf->getErrorPage(404) << std::endl;
	std::cout << std::endl;
	std::cout << "Location...: " << conf->getLocation("/cgi-bin").getRoot() << std::endl;
	return (0);
}

int	main(int ac, char** av) {
	std::string	serverConfigurationFilePath = "./configs/default.json";
	if (ac > 2)
		return (1);
	else if (ac == 2)
		serverConfigurationFilePath = av[1];
	ServerArray		servers(serverConfigurationFilePath);

	std::cout << "size: " << servers.GetSize() << std::endl;
	if (servers.GetSize() > 0)
		return (ws_testing(servers.GetServer(0)));
	return (0);
}
