#ifndef SERVER_HPP
# define SERVER_HPP


#include "ft_irc.hpp"
#include "Client.hpp"

class Server
{
    private:
    int server_fd;
    int port;
    std::string password;
    public:
        Server(int port, const std::string& password);
        ~Server();
        void run();
};
#endif