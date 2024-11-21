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
	this->_error = this->_servers.getError();
}

/* G. Getters */
int					Cluster::getError(void) const {
	return (this->_error);
}
ServerConfigArray&	Cluster::getServerConfigArray(void) {
	return (this->_servers);
}
ServerConfig&		Cluster::getServerConfig(const size_t& pos) {
	return (this->_servers.GetServer(pos));
}
size_t				Cluster::getServerConfigSize(void) const {
	return (this->_servers.GetSize());
}

/* PUB0. Core */
void	Cluster::run(void) {

	
/*
	pollfd	test[2];
	test[0].fd = STDERR_FILENO;
	test[0].events = POLL_IN;
	test[1].fd = STDIN_FILENO;
	test[1].events = POLL_IN;

	while (1) {
		Log::debug("RunPoll");
		int ret = poll(test, 1, 3000);
		Log::debug("State: " + stp_itoa(ret));
		if (ret > 0) {
			if (test[0].revents & POLLIN)
			Log::debug("Dados para leitura!");

			char*	bufff = new char[100];

			int bytes = read(STDIN_FILENO, bufff, 100);
			Log::debug("Bytes: " + stp_itoa(bytes));
			bufff[bytes] = '\0';
			Log::debug("Retur: " + std::string(bufff));

			delete[] bufff;
		}
	}
*/
}

/* PRI0. Private Functions */
void	Cluster::deepCopy(const Cluster& src) {
	this->_error = src._error;
	this->_servers = src._servers;
}