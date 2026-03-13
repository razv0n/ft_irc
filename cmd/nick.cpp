#include "../includes/server.hpp"
void Server::handleNick(int client_fd, const std::vector<std::string> &tokens)
{
    std::string nick = clientsFds[client_fd]->getNickOk() ? clientsFds[client_fd]->getNick() : "*";
    if (tokens.size() != 2)
        throw std::runtime_error(":ircserv 431 " + nick + " :No nickname given");
    if (clientsFds[client_fd]->getPassOk() == false)
        throw std::runtime_error(":ircserv 451 " + nick + " :You have not registered");
    if (clientsName.count(tokens[1]))
        throw std::runtime_error(":ircserv 433 " + nick + " " + tokens[1] + " :Nickname is already in use");
    if(clientsFds[client_fd]->getNickOk()) // can i if i want notify channels
        clientsName.erase(clientsFds[client_fd]->getNick());
    else
        clientsFds[client_fd]->setNickOk(true);
    clientsFds[client_fd]->setNick(tokens[1]);
    clientsName[tokens[1]] = clientsFds[client_fd];
}