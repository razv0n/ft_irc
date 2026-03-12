#include "../includes/server.hpp"

void Server::handleInvite(int client_fd, const std::vector<std::string> &tokens)
{
    if(!clientsFds[client_fd]->isRegistered())
    {
        std::string msg = "You are not registered\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(tokens.size() > 4 || tokens.size() < 3)
    {
        send(client_fd, "Usage: INVITE <nick> <#channel>\r\n", 30, 0);
        return;
    }
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
    std::string msg = ":" + clientsFds[client_fd]->getNick() + " INVITE " + invite_name + " " + channel_name + "\r\n";
    send(clientsName[invite_name]->getFd(), msg.c_str(), msg.length(), 0);
    channels[channel_name]->addInvite(clientsFds[client_fd]);
}