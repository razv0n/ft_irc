#include "../includes/server.hpp"

void Server::handleJoin(int client_fd, const std::vector<std::string> &tokens)
{
    if (clientsFds[client_fd]->isRegistered() == false)
    {
        std::string msg = "You are not registered\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if (tokens.size() < 2)
    {
        std::string msg = "Usage: JOIN <channel> [key]\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    std::string channel_name = tokens[1];
    if (channel_name[0] != '#')
    {
        std::string msg = "Invalid channel name\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    std::string key = "";
    if (tokens.size() > 2) {
        key = tokens[2]; 
    }
    if (channels.find(channel_name) == channels.end())
    {
        channels[channel_name] = new Channel(channel_name, clientsFds[client_fd]);
        if(!key.empty())
            channels[channel_name]->setKey(key);
    }
    else
    {
        if (channels[channel_name]->isKeySet() && channels[channel_name]->getKey() != key)
        {
            std::string msg = "Invalid key\r\n";
            send(client_fd, msg.c_str(), msg.length(), 0);
            return;
        }
        else if(channels[channel_name]->isInviteOnly() && !channels[channel_name]->getInvites().count(clientsFds[client_fd]))
        {
            std::string msg = ":ircserv " + clientsFds[client_fd]->getNick() + " " + channel_name + " :Cannot join channel (+i)\r\n";
            send(client_fd, msg.c_str(), msg.length(), 0);
            return;
        }
        if(channels[channel_name]->isLimitSet())
        {
            if(channels[channel_name]->getLimit() == channels[channel_name]->getCurrLimit())
            {
                std::string msg = ":ircserv " + clientsFds[client_fd]->getNick() + " " + channel_name + " :Cannot join channel (+l the channel is full)\r\n";
                send(client_fd, msg.c_str(), msg.length(), 0);
                return  ;
            }
            channels[channel_name]->incLimit();
        }
        channels[channel_name]->addClient(clientsFds[client_fd]);
    }
    std::string msg = "Welcome " + clientsFds[client_fd]->getNick() + "!\r\n";
    send(client_fd, msg.c_str(), msg.length(), 0);
}