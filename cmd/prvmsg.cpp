#include "../includes/server.hpp"

void Server::handlePrivmsg(int client_fd, const std::vector<std::string> &tokens)
{
    if(!clientsFds[client_fd]->isRegistered())
    {
        std::string msg = "You are not registered\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(tokens.size() > 3 || tokens.size() < 3)
    {
        send(client_fd, "Usage: PRIVMSG <target> <text>\r\n", 32, 0);
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
    channels[channel_name]->brodcastMsg(clientsFds[client_fd]->getNick() + " : " + tokens[2] + "\r\n", clientsFds[client_fd]);
    
}