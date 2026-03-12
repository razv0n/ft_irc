#include "../includes/server.hpp"

void Server::handleTopic(int client_fd, const std::vector<std::string> &tokens)
{
    isRegistered(clientsFds[client_fd]);
    size_t tokensSize = tokens.size();
    if(tokensSize > 3)
        throw std::runtime_error("Usage: TOPIC <#channel>");
    std::string channel_name = tokens[1];
    checkChannelName(channel_name);
    checkChannelExist(channel_name);
    checkIsMember(channel_name, clientsFds[client_fd], "you");
    if(tokensSize == 2)
    {
        std::string topic = channels[channel_name]->isTopicSet() ?  channels[channel_name]->getTopic(): "No topic is set";
        throw std::runtime_error("ircserv " + clientsFds[client_fd]->getNick() + " " + channel_name + " :" + topic);
    }
    else if (channels[channel_name]->isTopicProtected() && !channels[channel_name]->getOperators().count(clientsFds[client_fd]))
        throw std::runtime_error("you are not an operator inside " + channel_name);
    std::string topic = tokens[2];
    channels[channel_name]->setTopic(topic);
}