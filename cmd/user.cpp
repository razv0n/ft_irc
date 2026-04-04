
#include "../includes/server.hpp"
#include <stdexcept>

void Server::handleUser(int client_fd, const std::vector<std::string> &tokens)
{
    std::string nick = clientsFds[client_fd]->getNickOk() ? clientsFds[client_fd]->getNick() : "*";
    if (tokens.size() > 5 || tokens.size() < 5)
            throw std::runtime_error(":ircserv 461 " + nick + " USER :Not enough parameters");
    if (clientsFds[client_fd]->getUserOk())
        throw std::runtime_error(":ircserv 462 " + nick + " :You may not reregister");
    if (clientsFds[client_fd]->getNickOk() == false)
        throw std::runtime_error(":ircserv 451 " + nick + " :You have not registered");
    clientsFds[client_fd]->setUsername(tokens[1]);
    clientsFds[client_fd]->setRealname(tokens[4]);
    // clientsFds[client_fd]->setHostname(tokens[2]);
    clientsFds[client_fd]->setUserOk(true); // we dont need to check this USEROK mean the setregistred is ok :>
    clientsFds[client_fd]->setRegistered(true);
    // Send user modes
    throw std::runtime_error(":ircserv 001 " + clientsFds[client_fd]->getNick() + " :Welcome to ft_irc!");
}
