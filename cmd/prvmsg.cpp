#include "../includes/server.hpp"

void Server::handlePrivmsg(int client_fd, const std::vector<std::string> &tokens)
{
    isRegistered(clientsFds[client_fd]);
    if(tokens.size() != 3)
        throw std::runtime_error(":ircserv 461 " + clientsFds[client_fd]->getNick() + " PRIVMSG :Not enough parameters");
    std::string ChannelORclient = tokens[1];
    if(ChannelORclient[0] == '#')
    {
        checkChannelExist(ChannelORclient, clientsFds[client_fd]->getNick());
        checkIsMember(ChannelORclient, clientsFds[client_fd], clientsFds[client_fd]->getNick());
        channels[ChannelORclient]->brodcastMsg(":" + clientsFds[client_fd]->getNick() + " PRIVMSG " + ChannelORclient + " :" + tokens[2], clientsFds[client_fd]);
    }
    else
    {
        checkClientExist(clientsFds[client_fd]->getNick(),ChannelORclient);
        sendMsg(clientsName[tokens[1]]->getFd(), ":" + clientsFds[client_fd]->getNick() + "!" + clientsFds[client_fd]->getUsername() + "@host PRIVMSG " + ChannelORclient + " :" + tokens[2]);
    }
}