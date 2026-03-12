#include "../includes/server.hpp"
void Server::handleNick(int client_fd, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 2)
        throw std::runtime_error("Usage: NICK <nickname>");
    if (clientsFds[client_fd]->getPassOk() == false)
        throw std::runtime_error("You are not registered");
    if (clientsName.count(tokens[1]))
        throw std::runtime_error("This nickname is already use");
    if(clientsFds[client_fd]->getNickOk()) // can i if i want notify channels
        clientsName.erase(clientsFds[client_fd]->getNick());
    else
        clientsFds[client_fd]->setNickOk(true);
    clientsFds[client_fd]->setNick(tokens[1]);
    clientsName[tokens[1]] = clientsFds[client_fd];
}