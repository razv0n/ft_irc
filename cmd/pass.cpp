#include "../includes/server.hpp"
void Server::handlePass(int client_fd, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 2)
        throw std::runtime_error("Usage: PASS <password>");
    if (clientsFds[client_fd]->getPassOk())
        throw std::runtime_error("You are already registered");
    if (tokens[1] != password)
        throw std::runtime_error("Password is incorrect");
    clientsFds[client_fd]->setPassOk(true);
    throw std::runtime_error("the Password is correct");
}