#include "../includes/server.hpp"

int isValidNm(std::string strNm)
{
    char *end;
    long nm = strtol(strNm.c_str(), &end, 10);
    if(*end != '\0' || nm <= 0 || nm > INT_MAX || nm < INT_MIN)
        return 0;
    return nm;
}

void Server::handleMode(int client_fd, const std::vector<std::string> &tokens)
{
    isRegistered(clientsFds[client_fd]);
    if(tokens.size() < 3 || tokens.size() > 4)
        throw std::runtime_error("Usage: MODE <#channel> +/-mode [parameter]");
    std::string channel_name = tokens[1];
    std::string mode = tokens[2];
    checkChannelName(channel_name, clientsFds[client_fd]->getNick());
    checkChannelExist(channel_name);
    checkIsMember(channel_name, clientsFds[client_fd], "you");
    checkIsOperator(channel_name, clientsFds[client_fd]);
    if(mode[0] != '-' && mode[0] != '+')
        throw std::runtime_error("the mode is incorrect");
    if(tokens.size() == 3)
    {
        if(mode[1] == 'i')
            channels[channel_name]->setInviteOnly(mode[0] == '-' ? false: true);
        else if(mode[1] == 't')
            channels[channel_name]->setTopicProtected(mode[0] == '-' ? false: true);
        else if(mode[1] == 'k' && mode[0] == '-')
        {
            channels[channel_name]->setKey("");
            channels[channel_name]->setKeySet(false);
        }
        else if(mode[1] == 'l' && mode[0] == '-')
        {
            channels[channel_name]->setLimit(0);
            channels[channel_name]->setLimitSet(false);
        }
    }
    else
    {
        if(mode[1] == 'k')
        {
            channels[channel_name]->setKey(tokens[3]);
            channels[channel_name]->setKeySet(true);
        }
        else if(mode[1] == 'l')
        {
            int limit = isValidNm(tokens[3]);
            if(!limit)
                throw std::runtime_error("the number is incorrect");
            channels[channel_name]->setLimit(limit);
            channels[channel_name]->setLimitSet(true);
        }
        else if(mode[1] == 'o')
        {
            checkIsMember(channel_name, clientsName[tokens[3]], tokens[3]);
            if(mode[0] == '-')
                  channels[channel_name]->removeOperator(clientsName[tokens[3]]);
            else
                 channels[channel_name]->addOperator(clientsName[tokens[3]]);
        }
    }
    throw std::runtime_error("the mode is incorrect");
}