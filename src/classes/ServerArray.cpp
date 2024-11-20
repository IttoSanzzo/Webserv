#include "webserv.hpp"

/* C. Constructors */
ServerArray::~ServerArray(void) {
	if (this->servers != NULL)
		delete[] this->servers;
}
ServerArray::ServerArray(void) {
	this->servers = NULL;
	this->size = 0;
}
ServerArray::ServerArray(std::string& configurationFilePath) {
	this->servers = NULL;
	this->size = 0;
	JsonChildren 	serverChildren;
	try {
		JsonNode		serversConfigFile = JsonNode::TryParseJsonFromFile(configurationFilePath);
		serverChildren = serversConfigFile.TryGetChildren("servers");
		if (serverChildren.GetSize() == 0)
			throw (ServerConfig::ErrorException("There were no server configs found!"));
	} catch (const std::exception& ex) {
		std::cout << "Configuration File Error: ";
		std::cout << ex.what() << std::endl;
		exit (10);
	}
	try {
		this->size = serverChildren.GetSize();
		this->servers = new ServerConfig[this->size];
		for (size_t i = 0; i < this->size; ++i)
			this->servers[i] = ServerConfig(serverChildren.GetChildNode(i));
	} catch (const std::exception& ex) {
		std::cout << "Configuration File Error: ";
		std::cout << ex.what() << std::endl;
		exit (11);
	}
}

/* G. Getters */
ServerConfig*	ServerArray::GetServer(size_t pos) const {
	if (pos < this->size)
		return (this->servers + pos);
	return (NULL);
}
size_t	ServerArray::GetSize(void) const {
	return (this->size);
}