#include "../includes/server.hpp"
void Server::handlePass(int client_fd, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 2)
    {
        std::string msg = "Usage: PASS <password>\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if (clientsFds[client_fd]->getPassOk())
    {
        std::string msg = "You are already registered\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if (tokens[1] != password)
    {
        std::string msg = "Password is incorrect\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    clientsFds[client_fd]->setPassOk(true);
    send(client_fd,"the Password is correct\r\n",25,0);
}