#include "../includes/server.hpp"
void Server::handlePart(int client_fd, const std::vector<std::string> &tokens)
{
   isRegistered(clientsFds[client_fd]);
    if(tokens.size() > 2 || tokens.size() < 3)
        throw std::runtime_error("Usage: PART <channel> :<msg>\r\n");
    std::string channel_name = tokens[1];
    checkChannelName(channel_name);
    checkChannelExist(channel_name);
    checkIsMember(channel_name, clientsFds[client_fd], clientsFds[client_fd]->getNick());
    if(channels[channel_name]->getInvites().count(clientsFds[client_fd]))
        channels[channel_name]->removeInvite(clientsFds[client_fd]);
    if(channels[channel_name]->getOperators().count(clientsFds[client_fd]))
        channels[channel_name]->removeOperator(clientsFds[client_fd]);
    channels[channel_name]->removeClient(clientsFds[client_fd]);
    if(channels[channel_name]->channelEmpty())
    {
        delete channels[channel_name];
        channels.erase(channel_name);
    }
    else if(tokens.size() == 3)
    {
        std::string msg =  ":" + clientsFds[client_fd]->getNick() + "!"+ clientsFds[client_fd]->getUsername() + " PART "+ channel_name + " :" + tokens[2] + "\r\n";
        channels[channel_name]->brodcastMsg(msg, clientsFds[client_fd]);
    }
}