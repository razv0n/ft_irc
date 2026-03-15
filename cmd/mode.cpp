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
    if (tokens.size() == 2) {
        std::string target = tokens[1];
        if (target[0] == '#') { // It's a channel
            checkChannelExist(target, clientsFds[client_fd]->getNick());
            
            // Send the 324 reply (RPL_CHANNELMODEIS)
            // Just send +t or +nt for now so Irssi is happy
            std::string nick = clientsFds[client_fd]->getNick();
            std::string rpl = ":ircserv 324 " + nick + " " + target + " +nt";
            sendMsg(client_fd, rpl);
            return; 
        }
        // If it's a nick, you can just ignore it or handle user modes
        return;
    }
    if(tokens.size() < 3 || tokens.size() > 4)
        throw std::runtime_error(":ircserv 461 " + clientsFds[client_fd]->getNick() + " MODE :Not enough parameters");
    std::string channel_name = tokens[1];
    std::string mode = tokens[2];
    checkChannelName(channel_name, clientsFds[client_fd]->getNick());
    checkChannelExist(channel_name, clientsFds[client_fd]->getNick());
    checkIsMember(channel_name, clientsFds[client_fd], "you");
    checkIsOperator(channel_name, clientsFds[client_fd]);
    if(mode[0] != '-' && mode[0] != '+')
        throw std::runtime_error(":ircserv 472 " + clientsFds[client_fd]->getNick() + " " + mode + " :is unknown mode char to me");
    if(mode.length() < 2)
        throw std::runtime_error(":ircserv 472 " + clientsFds[client_fd]->getNick() + " " + mode + " :is unknown mode char to me");
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
                throw std::runtime_error(":ircserv 461 " + clientsFds[client_fd]->getNick() + " MODE :Invalid limit parameter");
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
        else
            throw std::runtime_error(":ircserv 472 " + clientsFds[client_fd]->getNick() + " " + mode + " :is unknown mode char to me");
    }
    std::string rpl = ":ircserv 324 " + clientsFds[client_fd]->getNick() + " " + channel_name + " " + mode;
    sendMsg(client_fd, rpl);
    channels[channel_name]->brodcastMsg(":" + clientsFds[client_fd]->getNick() + " MODE " + channel_name + " " + mode, NULL);
}