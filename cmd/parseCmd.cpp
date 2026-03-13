#include "../includes/server.hpp"

void Server::checkChannelName(std::string channel_name, std::string member_name)
{
    if(channel_name[0] != '#')
        throw std::runtime_error(":ircserv 403 " + member_name + " " + channel_name + " :No such channel");
}
void Server::checkIsMember(std::string channel_name, client *member, std::string member_name)
{
    if(!channels[channel_name]->isMember(member))
        throw std::runtime_error(":ircserv 442 " + member_name + " " + channel_name + " :You're not on that channel");
}
void Server::checkChannelExist(std::string channel_name)
{
    if(!channels.count(channel_name))
        throw std::runtime_error("the channel not found");
}
void Server::checkClientExist(std::string client_name)
{
    if(!clientsName.count(client_name))
        throw std::runtime_error("the client not found");
}
void Server::checkIsOperator(std::string channel_name, client *member)
{
    if(!channels[channel_name]->isOperator(member))
        throw std::runtime_error("you are not an operator inside this channel");
}
void Server::checkIsInvite(std::string channel_name, client *member)
{
    if(!channels[channel_name]->getInvites().count(member))
        throw std::runtime_error("you are not an invite list");
}
void Server::isRegistered(client *member)
{
    if(!member->isRegistered())
        throw std::runtime_error("You are not registered");
}