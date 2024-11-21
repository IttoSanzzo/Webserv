#include "webserv.hpp"


void infoAllServerSettings(ServerConfigArray& serverConfigs) {
	if (Log::getInfoState() == true) {
		for (size_t i = 0; i < serverConfigs.GetSize(); ++i)
			Log::info("Server Config[" + stp_itoa(i + 1) + "]\n" + serverConfigs.GetServer(i)->toString());
		std::cout << std::endl;
	}
}
void	infoServerConfig(ServerConfig* serverConfig) {
	Log::info("\tPort.......: " + stp_itoa(serverConfig->getPort()));
	Log::info("\tHost.......: " + stp_itoa(serverConfig->getHost()));
	Log::info("\tName.......: " + serverConfig->getServerName());
	Log::info("\tRoot.......: " + serverConfig->getRoot());
	Log::info("\tIndex......: " + serverConfig->getIndex());
	Log::info("\tAutoindex..: " + stp_btoa(serverConfig->getAutoindex()));
	Log::info("\tCliMaxSize.: " + stp_itoa(serverConfig->getClientMaxBodySize()));
	if (serverConfig->getErrorPagesMap().size() > 0) {
		Log::info("\tErrorPages-> ");
		for (std::map<short, std::string>::iterator i = serverConfig->getErrorPagesMap().begin(); i != serverConfig->getErrorPagesMap().end(); ++i)
			Log::info("\t\tErrorPage[" + stp_itoa(i->first) + "].: " + i->second);
	}
	if (serverConfig->getLocationsMap().size() > 0) {
		Log::info("\tLocations-> ");
		for (std::map<std::string, Location>::iterator i = serverConfig->getLocationsMap().begin(); i != serverConfig->getLocationsMap().end(); ++i)
			Log::info("\t\tLocation[" + i->first + "]");
	}
}

int	main(int ac, char** av) {
	std::string	serverConfigurationFilePath = "./configs/default.json";
	if (ac > 2)
		return (1);
	else if (ac == 2)
		serverConfigurationFilePath = av[1];
	ServerConfigArray	servers(serverConfigurationFilePath);
	infoAllServerSettings(servers);


	return (0);
}
