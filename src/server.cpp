#include "server.hpp"


Server::Server(int port, const std::string& password)
{
    server_fd = -1;
    this->port = port;
    this->password = password;
    
}


void Server::run()
{
    // TODO: implement the server loop
}

Server::~Server()
{
    if (server_fd != -1)
        close(server_fd);
}