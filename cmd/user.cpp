
#include "../includes/server.hpp"

void Server::handleUser(int client_fd, const std::vector<std::string> &tokens)
{
    if (tokens.size() > 5 || tokens.size() < 5)
    {
        std::string msg = "Usage: USER <username> <hostname> <servername> :<realname>\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        // std::cout << "[DEBUG] USER rejected: wrong number of parameters" << std::endl;
        return;
    }
    if (clientsFds[client_fd]->getUserOk())
    {
        std::string msg = "You are already registered\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if (clientsFds[client_fd]->getNickOk() == false)
    {
        std::string msg = "You are not registered\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    clientsFds[client_fd]->setUsername(tokens[1]);
    clientsFds[client_fd]->setRealname(tokens[4]);
    clientsFds[client_fd]->setUserOk(true); // we dont need to check this USEROK mean the setregistred is ok :>
    clientsFds[client_fd]->setRegistered(true);
    std::string msg = "Welcome " + clientsFds[client_fd]->getNick() + "!\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
}