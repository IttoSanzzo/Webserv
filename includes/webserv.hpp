#ifndef WEBSERV_HPP
# define WEBSERV_HPP

/* 00. Default Includes */
# include <iostream>
# include <sstream>
# include <fstream>
# include <string>
# include <cstring>
# include <ctime>
# include <cstdarg>

/* 01. Cpp Containers */
# include <map>
# include <set>
# include <vector>
# include <algorithm>
# include <iterator>
# include <list>
# include <utility>

/* 02. C Defaults */
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <dirent.h>
# include <poll.h>

/* 03. C Network */
# include <sys/socket.h>
# include <sys/select.h>
# include <arpa/inet.h>
# include <netinet/in.h>

/* 03. System */
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/time.h>

/* 1. Local Macros */
# include "Others/ws_macros.hpp"

/* 2. Classes Declaration */
class	ServerConfig;
class	ServerConfigArray;
class	Location;

/* 4. Internal Includes */
# include "../CppJsonHandler/includes/JsonHandler.hpp"
# include "Classes/Log.hpp"
# include "Classes/ServerConfig.hpp"
# include "Classes/Location.hpp"
# include "Classes/ServerConfigArray.hpp"
# include "Classes/Cluster.hpp"

/* 5. Others */
int				ws_inet_pton(int af, const char *src, void *dst);
in_addr_t		ws_inet_addr(const char *cp);
void			infoAllServerSettings(ServerConfigArray& serverConfigs);
void			infoServerConfig(ServerConfig* serverConfig);
size_t			stp_stringArrayLength(const std::string* str);
int				stp_countOccurrences(const std::string& input, char delimiter);
std::string*	stp_split(const std::string& inputString, char delimiter);
bool			stp_stringIsNumeric(const std::string& str);
bool			stp_stringIsNumericOrSpaced(const std::string& str);
bool			stp_checkSufix(const std::string& str, const std::string& sufix);
std::string		stp_itoa(const int& number);
std::string		stp_btoa(const bool& value);

#endif