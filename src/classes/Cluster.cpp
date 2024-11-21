#include "webserv.hpp"

/* C. Constructors */
Cluster::~Cluster(void) {}
Cluster::Cluster(const Cluster& src) {
	this->deepCopy(src);
}
Cluster&	Cluster::operator=(const Cluster& src) {
	if (this != &src)
		this->deepCopy(src);
	return (*this);
}
Cluster::Cluster(std::string& configurationFilePath) {
	this->_servers = ServerConfigArray(configurationFilePath);
}

/* G. Getters */
ServerConfigArray&	Cluster::GetServerConfigArray(void) {
	return (this->_servers);
}
ServerConfig&		Cluster::GetServerConfig(const size_t& pos) {
	return (this->_servers.GetServer(pos));
}
size_t			Cluster::GetServerConfigSize(void) const {
	return (this->_servers.GetSize());
}

/* PUB0. Core */
void	Cluster::run(void) {
	
}

/* PRI0. Private Functions */
void	Cluster::deepCopy(const Cluster& src) {
	this->_servers = src._servers;
}