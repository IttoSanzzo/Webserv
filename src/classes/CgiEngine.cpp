#include "webserv.hpp"

char** CgiEngine::_envp = NULL;
CgiEngine::~CgiEngine(void) {}
CgiEngine::CgiEngine(void) {
	this->_dataDirectory = "";
}
CgiEngine::CgiEngine(ServerConfig* serverConfig) {
	this->_dataDirectory = serverConfig->getDataDirectory();
	this->_binPaths = serverConfig->getCgiPaths();
}
CgiEngine::CgiEngine(const CgiEngine& src) {
	this->deepCopy(src);
}
CgiEngine&		CgiEngine::operator=(const CgiEngine& src) {
	if (this != &src)
		this->deepCopy(src);
	return (*this);
}
void			CgiEngine::setEnvp(char** envp) {
	CgiEngine::_envp = envp;
}
void			CgiEngine::setdataDirectory(const std::string& dataDirectory) {
	this->_dataDirectory = dataDirectory;
}
void			CgiEngine::setBinPaths(const std::map<std::string, std::string>& binPath) {
	this->_binPaths = binPath;
}
char**			CgiEngine::getEnvp(void) {
	return (CgiEngine::_envp);
}
std::string		CgiEngine::getDataDirectory(void) const {
	return (this->_dataDirectory);
}
std::string		CgiEngine::getBinPath(const std::string& extension) {
	return (this->_binPaths[extension]);
}
HttpResponse	CgiEngine::runCgi(const std::string& index, HttpRequest& request) {
	HttpResponse response;
	int	pipeFDs[2];
	pipe(pipeFDs);
	pid_t pid = fork();
	if (pid == 0)
		this->runFork(index, request, pipeFDs);
	int	statLoc;
	pid_t error = waitpid(pid, &statLoc, 0);
	if (error == -1) {
		response.setCode(501);
		return (response);
	}
	::close(pipeFDs[1]);
	int	returnCode = WEXITSTATUS(statLoc);
	if (returnCode > 0) {
		response.setCode(501);
		return (response);
	}
	response.setCode(200);
	response.setContent(this->readPipe(pipeFDs[0]));
	return (response);
}
void			CgiEngine::runFork(const std::string& index, HttpRequest& request, int pipeFDs[2]) {
	::close(pipeFDs[0]);
	dup2(pipeFDs[1], STDOUT_FILENO);
	::close(pipeFDs[1]);
	size_t slashPos = index.rfind('/');
	std::string	programName = ((slashPos != std::string::npos) ? (index.substr(slashPos + 1)) : (index));
	size_t extensionPos = programName.rfind('.');
	std::string extension = ((extensionPos != std::string::npos) ? (programName.substr(extensionPos)) : (""));
	if (extension != "")
		this->runExtension(index, extension, request);
	else
		this->runExtensionless(programName, extension, index, request);
	std::exit(20);
}
void			CgiEngine::runExtension(const std::string& index, const std::string& extension, HttpRequest& request) {
	char**	argv = this->formArgv(this->_binPaths[extension], this->_dataDirectory + index, request.getQueryParameters());
	execve(argv[0], argv, CgiEngine::_envp);
}
void			CgiEngine::runExtensionless(const std::string& programName, const std::string& extension, const std::string& index, HttpRequest& request) {
	(void)programName;
	(void)extension;
	(void)index;
	(void)request;
}
char**			CgiEngine::formArgv(const std::string& binary, const std::string& program, const std::map<std::string, std::string> queryParameters) {
	size_t totalParameters = queryParameters.size() * 2;
	char** argv = (char**)malloc((totalParameters + 3) * sizeof(char*));
	argv[0] = stp_strdup(binary);
	argv[1] = stp_strdup(program);
	size_t position = 1;
	for (std::map<std::string, std::string>::const_iterator i = queryParameters.begin(); i != queryParameters.end(); ++i) {
		argv[++position] = stp_strdup(i->first);
		argv[++position] = stp_strdup(i->second);
	}
	argv[++position] = NULL;
	return (argv);
}
std::string		CgiEngine::readPipe(const int& pipeFD) {
	std::string	pipeString = "";
	char buffer[2550];
	size_t bytesRead;
	do {
		bytesRead = read(pipeFD, buffer, sizeof(buffer) - 1);
		buffer[bytesRead] = '\0';
		pipeString += buffer;
	} while (bytesRead != 0);
	::close(pipeFD);
	return (pipeString);
}
void			CgiEngine::deepCopy(const CgiEngine& src) {
	this->_dataDirectory = src._dataDirectory;
	this->_binPaths = src._binPaths;
}