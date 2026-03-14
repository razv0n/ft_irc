#include "../includes/server.hpp"
void Server::handleKick(int client_fd, const std::vector<std::string> &tokens)
{
    isRegistered(clientsFds[client_fd]);
    if(tokens.size() > 4 || tokens.size() < 3)
        throw std::runtime_error(":ircserv 461 " + clientsFds[client_fd]->getNick() + " KICK :Not enough parameters");
    std::string channel_name = tokens[1];
    checkChannelName(channel_name, clientsFds[client_fd]->getNick());
    checkChannelExist(channel_name,clientsFds[client_fd]->getNick());
    // checkIsMember(channel_name, clientsFds[client_fd], "you");
    checkIsOperator(channel_name, clientsFds[client_fd]);
    std::string kick_name = tokens[2];
    checkIsMember(channel_name, clientsName[kick_name], kick_name);
    if(tokens[2] == clientsFds[client_fd]->getNick())
        throw std::runtime_error(":ircserv NOTICE " + clientsFds[client_fd]->getNick() + " :You cannot kick yourself. Please use the PART command to leave the channel");
    client *kicked_client = clientsName[kick_name];
    int kicked_fd = kicked_client->getFd();
    std::string msg = "";
    if(tokens.size() == 4)
        msg = tokens[3];
    msg = ":" + clientsFds[client_fd]->getNick() + "!"+ clientsFds[client_fd]->getUsername() + " KICK "+ channel_name + " " + kick_name + " :" + msg + "\r\n";
    sendMsg(kicked_fd, msg);
    channels[channel_name]->brodcastMsg(msg, kicked_client);
    if(channels[channel_name]->isOperator(kicked_client))
        channels[channel_name]->removeOperator(kicked_client);
    if(channels[channel_name]->isInviteOnly())
        channels[channel_name]->removeInvite(kicked_client);
    channels[channel_name]->removeClient(kicked_client);
}
