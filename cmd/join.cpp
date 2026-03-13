#include "../includes/server.hpp"
#include <stdexcept>

void Server::handleJoin(int client_fd, const std::vector<std::string> &tokens)
{
    isRegistered(clientsFds[client_fd]);
    if (tokens.size() < 2 || tokens.size() > 3)
        throw std::runtime_error(":ircserv 461 " + clientsFds[client_fd]->getNick() + " JOIN :Not enough parameters");
    std::string channel_name = tokens[1];
    checkChannelName(channel_name, clientsFds[client_fd]->getNick());
    std::string key = "";
    if (tokens.size() == 3)
        key = tokens[2]; 
    if (!channels.count(channel_name))
    {
        channels[channel_name] = new Channel(channel_name, clientsFds[client_fd]);
        if(!key.empty())
            channels[channel_name]->setKey(key);
    }
    else
    {
        if (channels[channel_name]->isKeySet() && channels[channel_name]->getKey() != key)
            throw std::runtime_error(":ircserv 475 " + clientsFds[client_fd]->getNick() + " " + channel_name + " :Cannot join channel (+k)");
        else if(channels[channel_name]->isInviteOnly() && !channels[channel_name]->getInvites().count(clientsFds[client_fd]))
            throw std::runtime_error(":ircserv 473 " + clientsFds[client_fd]->getNick() + " " + channel_name + " :Cannot join channel (+i)");
        if(channels[channel_name]->isLimitSet())
        {
            if(channels[channel_name]->getLimit() == channels[channel_name]->getCurrLimit())
                throw std::runtime_error(":ircserv 471 " + clientsFds[client_fd]->getNick() + " " + channel_name + " :Cannot join channel (+l)");
            channels[channel_name]->incLimit();
        }
        channels[channel_name]->addClient(clientsFds[client_fd]);
    }
    throw std::runtime_error(":ircserv 001 " + clientsFds[client_fd]->getNick() + " :Welcome to ft_irc!");
}