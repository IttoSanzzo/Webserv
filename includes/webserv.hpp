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
# include "Others/ws_dataStructures.hpp"
# include "Others/stplib.hpp"

/* 2. Classes Declaration */
class	ServerConfig;
class	ServerConfigArray;
class	Route;
class	HttpRequest;
class	HttpResponse;

/* 4. Internal Includes */
# include "../CppJsonHandler/includes/JsonHandler.hpp"
# include "Classes/Log.hpp"
# include "Classes/ServerConfig.hpp"
# include "Classes/Route.hpp"
# include "Classes/ServerConfigArray.hpp"
# include "Classes/Server.hpp"
# include "Classes/Cluster.hpp"
# include "Classes/HttpRequest.hpp"
# include "Classes/HttpResponse.hpp"
# include "Classes/RequestProcessor.hpp"

/* 5. Others */
int			ws_inet_pton(int af, const char *src, void *dst);
in_addr_t	ws_inet_addr(const char *cp);
std::string	ws_inet_ntoa(in_addr_t addr);
std::string	httpStatusCodeString(const int& code);
void		infoAllServerSettings(ServerConfigArray& serverConfigs);

#endif