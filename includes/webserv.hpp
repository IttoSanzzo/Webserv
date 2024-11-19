#ifndef WEBSERV_HPP
# define WEBSERV_HPP

/* Default Includes */

# include <iostream>
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

/* Local Macros */
# include "ws_macros.hpp"

/* Class Includes */
# include "ServerConfig.hpp"

/* Others */
int			ws_inet_pton(int af, const char *src, void *dst);
in_addr_t	ws_inet_addr(const char *cp);

#endif
