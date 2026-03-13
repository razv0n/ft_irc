#include "../includes/server.hpp"
void Server::handlePass(int client_fd, const std::vector<std::string> &tokens)
{
    std::string nick = clientsFds[client_fd]->getNickOk() ? clientsFds[client_fd]->getNick() : "*";
    if (tokens.size() != 2)
        throw std::runtime_error(":ircserv 461 " + nick + " PASS :Not enough parameters");
    if (clientsFds[client_fd]->getPassOk())
        throw std::runtime_error(":ircserv 462 " + nick + " :You may not reregister");
    if (tokens[1] != password)
        throw std::runtime_error(":ircserv 464 " + nick + " :Password incorrect");
    clientsFds[client_fd]->setPassOk(true);
}