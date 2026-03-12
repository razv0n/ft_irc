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
    if(!clientsFds[client_fd]->isRegistered())
    {
        std::string msg = "You are not registered\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(tokens.size() < 3 || tokens.size() > 4)
    {
        std::string msg = "Usage: MODE <#channel> +/-mode [parameter]\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    std::string channel_name = tokens[1];
    std::string mode = tokens[2];
    if(channel_name[0] != '#')
    {
        std::string msg = "Invalid channel name\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(!channels[channel_name]->isMember(clientsFds[client_fd]))
    {
        std::string msg = "you are not a member inside this channel\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(!channels[channel_name]->isOperator(clientsFds[client_fd]))
    {
        std::string msg = "you are not an operator inside this channel\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(mode.size() != 3 || mode[0] != '-' || mode[0] != '+')
    {
        std::string msg = "the mode is incorrect\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    switch (mode[1])
    {
    case 'i':
        channels[channel_name]->setInviteOnly(mode[0] == '-' ? false: true);
        return;
    case 't':
        channels[channel_name]->setTopicProtected(mode[0] == '-' ? false: true);
        return;
    case 'k':
        if(tokens.size() == 4 && mode[0] == '+')
        {
            std::string new_key = tokens[3];
            channels[channel_name]->setKey(new_key);
            channels[channel_name]->setKeySet(true);
            return;
        }
        else if(tokens.size() == 3 && mode[0] == '-')
        {
            channels[channel_name]->setKey("");
            channels[channel_name]->setKeySet(false);
            return;
        }
        break;
    case 'o':
        if(!channels[channel_name]->isMember(clientsName[tokens[3]]))
        {
            std::string msg = "the " + tokens[3] + " is not a member inside this channel \r\n";
            send(client_fd, msg.c_str(), msg.length(), 0);
            return;
        }
        if(tokens.size() == 4)
        {
            if(mode[0] == '-')
                channels[channel_name]->removeOperator(clientsName[tokens[3]]);
            else
                channels[channel_name]->addOperator(clientsName[tokens[3]]);
            return;
        }
        break;
    case 'l':
        if(tokens.size() == 4 && mode[0] == '+')
        {
            std::string new_limit = tokens[3];
            if(int limit = isValidNm(new_limit))
            {
                channels[channel_name]->setLimit(limit); 
                channels[channel_name]->setLimitSet(true);
                return;
            }
            else
            {
                std::string msg = "the number is incorrect\r\n";
                send(client_fd, msg.c_str(), msg.length(), 0);
                return;         
            }
        }
        else if(tokens.size() == 3 && mode[0] == '-')
        {
            channels[channel_name]->setLimit(0);
            channels[channel_name]->setLimitSet(false);
            return;
        }
        break;
    }
    std::string msg = "the mode is incorrect\r\n";
    send(client_fd, msg.c_str(), msg.length(), 0);
}