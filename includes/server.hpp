#ifndef SERVER_HPP
# define SERVER_HPP


#include "ft_irc.hpp"
#include "client.hpp"
#include <vector>
#include <map>

class Server
{
    private:
    int server_fd;
    int port;
    std::string password;
    std::vector<struct pollfd> poll_fds;
    std::map<int, client> clients;
    public:
        Server(int port, const std::string& password);
        ~Server();
        void run();
};
#endif