#include "../includes/server.hpp"
void Server::handleNick(int client_fd, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 2)
    {
        std::string msg = "Usage: NICK <nickname>\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if (clientsFds[client_fd]->getPassOk() == false)
    {
        std::string msg = "You are not registered\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if (clientsName.count(tokens[1]))
    {
        send(client_fd, "This nickname is already use\r\n", 30, 0);
        return;
    }
    if(clientsFds[client_fd]->getNickOk()) // can i if i want notify channels
        clientsName.erase(clientsFds[client_fd]->getNick());
    else
        clientsFds[client_fd]->setNickOk(true);
    clientsFds[client_fd]->setNick(tokens[1]);
    clientsName[tokens[1]] = clientsFds[client_fd];
}