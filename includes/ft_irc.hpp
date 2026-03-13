#ifndef FT_IRC_HPP
# define FT_IRC_HPP

#include <poll.h>
#include <string>
#include <cstring>
#include <set>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <limits.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <cerrno>
#include "server.hpp"
#include "client.hpp"
#include <csignal>

extern bool g_server_shutdown;

#endif
