#include "webserv.hpp"


void infoAllServerSettings(ServerConfigArray& serverConfigs) {
	Log::debug("Giving Server Info");
	if (Log::getInfoState() == true)
		for (size_t i = 0; i < serverConfigs.getSize(); ++i)
			Log::info("Server Config[" + stp_itoa(i + 1) + "]\n" + serverConfigs.getServer(i).toString());
	Log::debug("Server Info Given");
}

int	main(int ac, char** av) {
	std::string	serverConfigurationFilePath = DEFAULT_CONFIG;
	if (MAX_CLI_SIZE > 4000) {
		Log::error("You should not set Max Client Body Size above 4000!");
		return (1);
	}
	else if (ac > 2) {
		Log::error("Too many arguments!");
		return (2);
	}
	else if (ac == 2)
		serverConfigurationFilePath = av[1];
	Cluster	cluster(serverConfigurationFilePath);
	if (cluster.getError() > 0)
		return (cluster.getError());
	infoAllServerSettings(cluster.getServerConfigArray());
	try {
		cluster.runCluster();
	} catch (const std::exception& ex) {
		Log::error(ex);
		return (12);
	}
	return (0);
}