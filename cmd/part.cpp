#include "../includes/server.hpp"
void Server::handlePart(int client_fd, const std::vector<std::string> &tokens)
{
    if(!clientsFds[client_fd]->isRegistered())
   {
       std::string msg = "You are not registered\r\n";
       send(client_fd, msg.c_str(), msg.length(), 0);
       return;
   }
    if(tokens.size() > 2 || tokens.size() < 3)
    {
        send(client_fd, "Usage: PART <channel> :<msg>\r\n", 30, 0);
        return;
    }
    std::string channel_name = tokens[1];
    if(channel_name[0] != '#')
    {
        std::string msg = "Invalid channel name\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(!channels.count(channel_name))
    {
        std::string msg = "the channel not found\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(!channels[channel_name]->isMember(clientsFds[client_fd]))
    {
        std::string msg = "you are not a member inside this channel\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
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