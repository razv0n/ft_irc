#include "../includes/server.hpp"
void Server::handleKick(int client_fd, const std::vector<std::string> &tokens)
{
    if(!clientsFds[client_fd]->isRegistered())
    {
        std::string msg = "You are not registered\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(tokens.size() > 4 || tokens.size() < 3)
    {
        send(client_fd, "Usage: KICK <channel> <nick> :<msg>\r\n", 30, 0);
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
    if(!channels[channel_name]->isOperator(clientsFds[client_fd]))
    {
        std::string msg = "you are not an operator inside this channel\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    std::string kick_name = tokens[2];
    if(!channels[channel_name]->isMember(clientsName[tokens[2]]))
    {
        std::string msg = "the " + tokens[2] +"is not a member inside this channel\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(tokens[2] == clientsFds[client_fd]->getNick())
    {
        std::string msg = "really nega :>\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(channels[channel_name]->isOperator(clientsName[kick_name]))
        channels[channel_name]->removeOperator(clientsName[kick_name]);
    if(channels[channel_name]->isInviteOnly())
        channels[channel_name]->removeInvite(clientsName[kick_name]);
    channels[channel_name]->removeClient(clientsName[kick_name]);
    std::string msg = "";
    if(tokens.size() == 4)
        msg = tokens[3];
    msg = ":" + clientsFds[client_fd]->getNick() + "!"+ clientsFds[client_fd]->getUsername() + " KICK "+ channel_name + kick_name + " :" + msg +"\r\n";
    channels[channel_name]->brodcastMsg(msg, NULL);
}
