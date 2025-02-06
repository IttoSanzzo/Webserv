#ifndef CGIENGINE_HPP
# define CGIENGINE_HPP

#include "webserv.hpp"

class CgiEngine {
	private:
		static char**						_envp;
		std::string							_dataDirectory;
		std::map<std::string, std::string>	_binPaths;
	public:
		~CgiEngine(void);
		CgiEngine(void);
		CgiEngine(ServerConfig* serverConfig);
    	CgiEngine(const CgiEngine& src);
    	CgiEngine&		operator=(const CgiEngine& src);
		static void		setEnvp(char** envp);
		void			setdataDirectory(const std::string& dataDirectory);
		void			setBinPaths(const std::map<std::string, std::string>& binPath);
		static char**	getEnvp(void);
		std::string 	getDataDirectory(void) const;
		std::string		getBinPath(const std::string& extension);
		HttpResponse	runCgi(const std::string& index, HttpRequest& request);
	private:
		void			runFork(const std::string& index, HttpRequest& request, int pipeFDs[2]);
		void			runExtension(const std::string& index, const std::string& extension, HttpRequest& request);
		void			runExtensionless(const std::string& programName, const std::string& extension, const std::string& index, HttpRequest& request);
		char**			formArgv(const std::string& binary, const std::string& program, const std::map<std::string, std::string> queryParameters);
		std::string		readPipe(const int& pipeFD);
		void			deepCopy(const CgiEngine& src);
};

#endif