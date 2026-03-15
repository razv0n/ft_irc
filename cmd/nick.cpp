#include "../includes/server.hpp"
void Server::handleNick(int client_fd, const std::vector<std::string> &tokens)
{
    std::string old_nick = clientsFds[client_fd]->getNickOk() ? clientsFds[client_fd]->getNick() : "*";
    if (tokens.size() != 2)
        throw std::runtime_error(":ircserv 431 " + old_nick + " :No nickname given");
    if (clientsFds[client_fd]->getPassOk() == false)
        throw std::runtime_error(":ircserv 451 " + old_nick + " :You have not registered");
    if (clientsName.count(tokens[1]))
        throw std::runtime_error(":ircserv 433 " + old_nick + " " + tokens[1] + " :Nickname is already in use");
    std::string new_nick = tokens[1];
    if(clientsFds[client_fd]->getNickOk())
    {
        std::map<std::string, Channel *>::iterator it = channels.begin();
        while (it != channels.end())
        {
            if ((it->second)->isMember(clientsFds[client_fd]))
                (it->second)->brodcastMsg(":" + old_nick + " NICK :" + new_nick, NULL);
            it++;
        }
        clientsName.erase(old_nick);
    }
    else
        clientsFds[client_fd]->setNickOk(true);
    clientsFds[client_fd]->setNick(new_nick);
    clientsName[new_nick] = clientsFds[client_fd];
}