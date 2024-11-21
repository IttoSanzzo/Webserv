#include "webserv.hpp"

/* C. Constructors */
ServerConfigArray::~ServerConfigArray(void) {
	if (this->serverConfigs != NULL)
		delete[] this->serverConfigs;
}
ServerConfigArray::ServerConfigArray(void) {
	this->serverConfigs = NULL;
	this->size = 0;
}
ServerConfigArray::ServerConfigArray(std::string& configurationFilePath) {
	this->serverConfigs = NULL;
	this->size = 0;
	JsonChildren 	serverChildren;
	try {
		JsonNode		serversConfigFile = JsonNode::TryParseJsonFromFile(configurationFilePath);
		serverChildren = serversConfigFile.TryGetChildren("servers");
		if (serverChildren.GetSize() == 0)
			throw (ServerConfig::ErrorException("There were no server configs found!"));
	} catch (const std::exception& ex) {
		Log::error("Configuration File Error: ");
		std::cout << ex.what() << std::endl;
		exit (10);
	}
	try {
		this->size = serverChildren.GetSize();
		this->serverConfigs = new ServerConfig[this->size];
		for (size_t i = 0; i < this->size; ++i)
			this->serverConfigs[i] = ServerConfig(serverChildren.GetChildNode(i));
	} catch (const std::exception& ex) {
		Log::error("Configuration File Error: ");
		std::cout << ex.what() << std::endl;
		exit (11);
	}
}

/* G. Getters */
ServerConfig*	ServerConfigArray::GetServer(size_t pos) const {
	if (pos < this->size)
		return (this->serverConfigs + pos);
	return (NULL);
}
size_t	ServerConfigArray::GetSize(void) const {
	return (this->size);
}