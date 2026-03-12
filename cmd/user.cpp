
#include "../includes/server.hpp"
#include <stdexcept>

void Server::handleUser(int client_fd, const std::vector<std::string> &tokens)
{
    if (tokens.size() > 5 || tokens.size() < 5)
        throw std::runtime_error("Usage: USER <username> <hostname> <servername> :<realname>");
    if (clientsFds[client_fd]->getUserOk())
        throw std::runtime_error("You are already registered");
    if (clientsFds[client_fd]->getNickOk() == false)
        throw std::runtime_error("You are not registered");
    clientsFds[client_fd]->setUsername(tokens[1]);
    clientsFds[client_fd]->setRealname(tokens[4]);
    clientsFds[client_fd]->setUserOk(true); // we dont need to check this USEROK mean the setregistred is ok :>
    clientsFds[client_fd]->setRegistered(true);
    throw std::runtime_error("Welcome " + clientsFds[client_fd]->getNick() + "!");
}