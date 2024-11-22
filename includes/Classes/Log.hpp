#ifndef LOG_HPP
# define LOG_HPP

#include "webserv.hpp"

typedef enum e_LogType {
	Log = 0,
	Info = 1,
	Warning = 2,
	Error = 3,
	Debug = 4
} LogType;

class Log {
	private:
		static const bool	key_log = LOG_LOG;
		static const bool	key_info = LOG_INFO;
		static const bool	key_warning = LOG_WARNING;
		static const bool	key_error = LOG_ERROR;
		static const bool	key_debug = LOG_DEBUG;
	/* C. Constructors */
		Log(void);
    	Log(const Log&);
    	Log&	operator=(const Log&);
	/* G. Getters */
	public:
		static bool	getLogState(void);
		static bool	getInfoState(void);
		static bool	getWarningState(void);
		static bool	getErrorState(void);
		static bool	getDebugState(void);
	/* PUB0. Core */
		static void	log(const std::string& message);
		static void	info(const std::string& message);
		static void	warning(const std::string& message);
		static void	error(const std::string& message);
		static void	error(const std::exception& ex);
		static void	debug(const std::string& message);
		static void	omni(const LogType& type, const std::string& message);
};

#endif