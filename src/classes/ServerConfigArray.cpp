#include "webserv.hpp"

/* C. Constructors */
ServerConfigArray::~ServerConfigArray(void) {}
ServerConfigArray::ServerConfigArray(void) {}
ServerConfigArray::ServerConfigArray(const ServerConfigArray& src) {
	this->deepCopy(src);
}
ServerConfigArray&	ServerConfigArray::operator=(const ServerConfigArray& src) {
	if (this != &src)
		this->deepCopy(src);
	return (*this);
}
ServerConfigArray::ServerConfigArray(std::string& configurationFilePath) {
	JsonChildren serverChildren;
	try {
		JsonNode serversConfigFile = JsonNode::TryParseJsonFromFile(configurationFilePath);
		serverChildren = serversConfigFile.TryGetChildren("servers");
		if (serverChildren.GetSize() == 0)
			throw (ServerConfig::ErrorException("There were no server configs found!"));
	} catch (const std::exception& ex) {
		Log::error("Configuration File Error: ");
		std::cout << ex.what() << std::endl;
		exit (10);
	}
	try {
		for (size_t i = 0; i < serverChildren.GetSize(); ++i)
			this->serverConfigs.push_back(ServerConfig(serverChildren.GetChildNode(i)));
	} catch (const std::exception& ex) {
		Log::error("Configuration File Error: ");
		std::cout << ex.what() << std::endl;
		exit (11);
	}
}

/* G. Getters */
ServerConfig&	ServerConfigArray::GetServer(size_t pos) {
	return	(this->serverConfigs[pos]);
}
size_t			ServerConfigArray::GetSize(void) const {
	return (this->serverConfigs.size());
}

/* PRI0. Private Functions */
void	ServerConfigArray::deepCopy(const ServerConfigArray& src) {
	this->serverConfigs = src.serverConfigs;
}