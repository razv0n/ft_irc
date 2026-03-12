#include "../includes/server.hpp"

void Server::handlePrivmsg(int client_fd, const std::vector<std::string> &tokens)
{
    isRegistered(clientsFds[client_fd]);
    if(tokens.size() > 3 || tokens.size() < 3)
        throw std::runtime_error("Usage: PRIVMSG <target> <text>");
    std::string ChannelORclient = tokens[1];
    if(ChannelORclient[0] == '#')
    {
        checkChannelExist(ChannelORclient);
        checkIsMember(ChannelORclient, clientsFds[client_fd], "you");
        channels[ChannelORclient]->brodcastMsg(clientsFds[client_fd]->getNick() + " : " + tokens[2] + "\r\n", clientsFds[client_fd]);
    }
    else
    {
        checkClientExist(ChannelORclient);
        sendMsg(clientsName[tokens[2]]->getFd(), clientsFds[client_fd]->getNick() + " PRIVMSG " + ChannelORclient + " :" + tokens[2]);
    }
}