#include "../includes/server.hpp"

void Server::handleInvite(int client_fd, const std::vector<std::string> &tokens)
{
    if(!clientsFds[client_fd]->isRegistered())
        throw std::runtime_error("You are not registered");
    if(tokens.size() > 4 || tokens.size() < 3)
       throw std::runtime_error("Usage: INVITE <nick> <#channel>");
    std::string channel_name = tokens[2];
    std::string invite_name = tokens[1];

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
    if(!channels[channel_name]->isOperator(clientsFds[client_fd]))
    {
        std::string msg = "you are not an operator inside this channel\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(!channels[channel_name]->isMember(clientsName[invite_name]))
    {
        std::string msg = "the " + invite_name +"is not a member inside this channel\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    channels[channel_name]->addInvite(clientsFds[client_fd]);
    throw std::runtime_error(":" + clientsFds[client_fd]->getNick() + " INVITE " + invite_name + " " + channel_name);
}