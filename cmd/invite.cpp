#include "../includes/server.hpp"

void Server::handleInvite(int client_fd, const std::vector<std::string> &tokens)
{
    isRegistered(clientsFds[client_fd]);
    if(tokens.size() != 3)
       throw std::runtime_error("Usage: INVITE <nick> <#channel>");
    std::string channel_name = tokens[2];
    std::string invite_name = tokens[1];

    checkChannelName(channel_name);
    checkChannelExist(channel_name);
    checkIsOperator(channel_name, clientsFds[client_fd]);
    checkClientExist(invite_name);
    channels[channel_name]->addInvite(clientsName[invite_name]);
    std::string msg = ":" + clientsFds[client_fd]->getNick() + " INVITE " + invite_name + " " + channel_name + "\r\n";
    send(clientsName[invite_name]->getFd(), msg.c_str(), msg.length(), 0);
}