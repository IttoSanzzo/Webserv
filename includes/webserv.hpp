#ifndef WEBSERV_HPP
# define WEBSERV_HPP

/* 0. Default Includes */
# include <iostream>
# include <algorithm>
# include <fcntl.h>
# include <cstring>
# include <string> 
# include <unistd.h>
# include <dirent.h>
# include <sstream>

# include <cstdlib>
# include <fstream>
# include <sstream>
# include <cctype>
# include <ctime>
# include <cstdarg>

# include <map>
# include <set>
# include <vector>
# include <algorithm>
# include <iterator>
# include <list>

# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <unistd.h>
# include <signal.h>

# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/select.h>
# include <arpa/inet.h>

/* 1. Local Macros */
# include "ws_macros.hpp"

/* 2. Classes Declaration */
class	ServerConfig;
class	Location;

/* 4. Internal Includes */
# include "../CppJsonHandler/includes/JsonHandler.hpp"
# include "ServerConfig.hpp"
# include "Location.hpp"
# include "ServerArray.hpp"

/* 5. Others */
int				ws_inet_pton(int af, const char *src, void *dst);
in_addr_t		ws_inet_addr(const char *cp);
size_t			stp_stringArrayLength(const std::string* str);
int				stp_countOccurrences(const std::string& input, char delimiter);
std::string*	stp_split(const std::string& inputString, char delimiter);
bool			stp_stringIsNumeric(const std::string& str);
bool			stp_stringIsNumericOrSpaced(const std::string& str);
bool			stp_checkSufix(const std::string& str, const std::string& sufix);

#endif