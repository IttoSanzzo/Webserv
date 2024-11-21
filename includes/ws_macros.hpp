#ifndef WS_MACROS_HPP
# define WS_MACROS_HPP

/* 0. Number Macros */
# define MAX_CLI_SIZE	30000000
# define MAX_URL_SIZE	4096
# define MAX_PORT_NUM	65500

/* 10. Log Sets */
# define LOG_LOG			true
# define LOG_INFO			true
# define LOG_WARNING		true
# define LOG_ERROR			true
# define LOG_DEBUG			true
/* 11. Log Colors */
# define LOGCOLOR_LOG		"\033[38;5;111m"
# define LOGCOLOR_INFO		"\033[38;5;33m"
# define LOGCOLOR_WARNING	"\033[38;5;208m"
# define LOGCOLOR_ERROR		"\033[38;5;196m"
# define LOGCOLOR_DEBUG		"\033[38;5;229m"

/* 2. Errors Messages */
# define ERR_PORT		"Wrong syntax: port"
# define ERR_HOST		"Wrong syntax: host"
# define ERR_AUTOINDEX	"Wrong syntax: autoindex"
# define ERR_MAXBODY	"Wrong syntax: client_max_body_size"
# define ERR_EXCE		"\033[38;5;196mSERVER CONFIG ERROR : \033[0m"

#endif
