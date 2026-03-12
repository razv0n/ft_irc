#include "../includes/server.hpp"

void Server::handleTopic(int client_fd, const std::vector<std::string> &tokens)
{
    if(!clientsFds[client_fd]->isRegistered())
    {
        std::string msg = "You are not registered\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    size_t tokensSize = tokens.size();
    if(tokensSize > 3)
    {
        std::string msg = "Usage: TOPIC <#channel>\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    std::string channel_name = tokens[1];
    if(!channels[channel_name]->isMember(clientsFds[client_fd]))
    {
        std::string msg = "you are not a member inside this channel\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(channel_name[0] != '#')
    {
        std::string msg = "Invalid channel name\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(tokensSize == 2)
    {
        std::string topic = channels[channel_name]->isTopicSet() ?  channels[channel_name]->getTopic(): "No topic is set";
        std::string msg = "ircserv " + clientsFds[client_fd]->getNick() + " " + channel_name + " :" + topic + "\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    else if (channels[channel_name]->isTopicProtected() && !channels[channel_name]->getOperators().count(clientsFds[client_fd]))
    {
        std::string msg = "ircserv " + clientsFds[client_fd]->getNick() + " you are not an operator inside " + channel_name + "\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    std::string topic = tokens[2];
    channels[channel_name]->setTopic(topic);
}