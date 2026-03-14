#include "../includes/server.hpp"

void Server::checkChannelName(std::string channel_name, std::string member_name)
{
    if(channel_name[0] != '#')
        throw std::runtime_error(":ircserv 403 " + member_name + " " + channel_name + " :No such channel");
}
void Server::checkIsMember(std::string channel_name, client *member, std::string member_name)
{
    if(!channels[channel_name]->isMember(member))
        throw std::runtime_error(":ircserv 442 " + member_name + " " + channel_name + " :not on that channel");
}
void Server::checkChannelExist(std::string channel_name, std::string member_name)
{
    if(!channels.count(channel_name))
        throw std::runtime_error(":ircserv 403 " + member_name + " " + channel_name + " :No such channel");
}
void Server::checkClientExist(std::string target_name, std::string requester_name)
{
    if(!clientsName.count(target_name))
        throw std::runtime_error(":ircserv 401 " + requester_name + " " + target_name + " :No such nick/channel");
}
void Server::checkIsOperator(std::string channel_name, client *member)
{
    if(!channels[channel_name]->isOperator(member))
        throw std::runtime_error(":ircserv 482 " + member->getNick() + " " + channel_name + "  :You're not channel operator");
}
void Server::checkIsInvite(std::string channel_name, client *member)
{
    if(!channels[channel_name]->getInvites().count(member))
        throw std::runtime_error(":ircserv 473 " + member->getNick() + " " + channel_name + "  :Cannot join channel (+i)");

}
void Server::isRegistered(client *member)
{
    if(!member->isRegistered())
        throw std::runtime_error(":ircserv 451 * :You have not registered");
}