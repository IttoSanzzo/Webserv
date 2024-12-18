#include "webserv.hpp"

ServerConfigArray::~ServerConfigArray(void) {}
ServerConfigArray::ServerConfigArray(void) {
	this->_error = 0;
}
ServerConfigArray::ServerConfigArray(const ServerConfigArray& src) {
	this->deepCopy(src);
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
		this->_error = 10;
		return ;
	}
	try {
		for (size_t i = 0; i < serverChildren.GetSize(); ++i)
			this->_serverConfigs.push_back(ServerConfig(serverChildren.GetChildNode(i)));
	} catch (const std::exception& ex) {
		Log::error("Configuration File Error: ");
		std::cout << ex.what() << std::endl;
		this->_error = 11;
		return ;
	}
	this->_error = 0;
}
ServerConfigArray&	ServerConfigArray::operator=(const ServerConfigArray& src) {
	if (this != &src)
		this->deepCopy(src);
	return (*this);
}
int					ServerConfigArray::getError(void) const {
	return (this->_error);
}
ServerConfig&		ServerConfigArray::getServer(size_t pos) {
	return	(this->_serverConfigs[pos]);
}
size_t				ServerConfigArray::getSize(void) const {
	return (this->_serverConfigs.size());
}
void				ServerConfigArray::deepCopy(const ServerConfigArray& src) {
	this->_serverConfigs = src._serverConfigs;
	this->_error = src._error;
}