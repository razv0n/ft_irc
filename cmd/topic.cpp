#include "../includes/server.hpp"

void Server::handleTopic(int client_fd, const std::vector<std::string> &tokens)
{
    isRegistered(clientsFds[client_fd]);
    size_t tokensSize = tokens.size();
    if(tokensSize < 2 || tokensSize > 3)
        throw std::runtime_error(":ircserv 461 " + clientsFds[client_fd]->getNick() + " TOPIC :Not enough parameters");
    std::string channel_name = tokens[1];
    checkChannelName(channel_name, clientsFds[client_fd]->getNick());
    checkChannelExist(channel_name, clientsFds[client_fd]->getNick());
    checkIsMember(channel_name, clientsFds[client_fd], "you");
    if(tokensSize == 2)
    {
        std::string topic = channels[channel_name]->isTopicSet() ?  channels[channel_name]->getTopic(): "No topic is set";
        if (channels[channel_name]->isTopicSet())
            throw std::runtime_error(":ircserv 332 " + clientsFds[client_fd]->getNick() + " " + channel_name + " :" + topic);
        throw std::runtime_error(":ircserv 331 " + clientsFds[client_fd]->getNick() + " " + channel_name + " :No topic is set");
    }
    else if (channels[channel_name]->isTopicProtected() && !channels[channel_name]->getOperators().count(clientsFds[client_fd]))
        throw std::runtime_error(":ircserv 482 " + clientsFds[client_fd]->getNick() + " " + channel_name + " :You're not channel operator");
    std::string topic = tokens[2];
    channels[channel_name]->setTopic(topic);
}