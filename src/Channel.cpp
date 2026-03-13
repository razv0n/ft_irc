#include "../includes/Channel.hpp"
#include "../includes/client.hpp"
Channel::Channel(const std::string& name, client* creator)
{
    this->name = name;
    this->topic = "";
    this->members.insert(creator);
    this->operators.insert(creator);
    this->invites.clear(); // the purpose of this is to make sure that the channel is not invite only by default
    this->is_invite_only = false;
    this->is_topic_set = false;
    this->is_topic_protected = false;
    this->is_limit_set = false;
    this->limit = 0;
    this->currentLimit = 0;
    this->is_key_set = false;
    this->key = "";
}

void Channel::addClient(client* member)
{
    if (!members.count(member))
        members.insert(member);
}
void Channel::incLimit()
{
    currentLimit++;
}
int Channel::getCurrLimit()
{
    return currentLimit;
}
void Channel::removeClient(client* member)
{
    std::set<client *>::iterator it = members.find(member); 
    if (it != members.end()) 
        members.erase(it);
}
Channel::~Channel()
{

}
void Channel::addOperator(client* member)
{
    this->operators.insert(member);
}
bool Channel::isOperator(client* member)
{
    return operators.count(member);
}

void Channel::removeOperator(client* member)
{
    std::set<client *>::iterator it = operators.find(member);
        if (it != operators.end())
            operators.erase(it);
}

void Channel::addInvite(client* member)
{
    this->invites.insert(member);
}

bool Channel::channelEmpty()
{
    return members.empty();
}

void Channel::removeInvite(client* member)
{
    std::set<client *>::iterator it = invites.find(member);
        if (it != invites.end())
            this->invites.erase(it);
}

void Channel::setTopic(const std::string& topic)
{
    this->topic = topic;
    this->is_topic_set = true;
}
void sendMsg(int member_fd, std::string msg)
{
    msg += "\r\n";
    send(member_fd, msg.c_str(), msg.length(), 0);
}

void Channel::setKey(const std::string& key)
{
    this->key = key;
    this->is_key_set = true;
}

void Channel::setLimit(int limit)
{
    this->limit = limit;
    this->is_limit_set = true;
}

void Channel::setInviteOnly(bool inviteOnly) // set/remove MODE +i/-i
{
    this->is_invite_only = inviteOnly;
}

void Channel::setTopicProtected(bool topicProtected) // set/remove MODE +t/-t
{
    this->is_topic_protected = topicProtected;
}

void Channel::setLimitSet(bool limitSet) // set/remove MODE +l/-l
{
    this->is_limit_set = limitSet;
}

void Channel::setKeySet(bool keySet) // set/remove MODE +k/-k
{
    this->is_key_set = keySet;
}

std::string Channel::getName() const
{
    return this->name;
}

std::string Channel::getTopic() const
{
    return this->topic;
}

std::set<client*> Channel::getClients() const
{
    return this->members;
}

std::set<client*> Channel::getOperators() const
{
    return this->operators;
}

std::set<client*> Channel::getInvites() const
{
    return this->invites;
}

bool Channel::isInviteOnly() const
{
    return this->is_invite_only;
}

bool Channel::isTopicSet() const
{
    return this->is_topic_set;
}

bool Channel::isTopicProtected() const
{
    return this->is_topic_protected;
}

bool Channel::isLimitSet() const
{
    return this->is_limit_set;
}

bool Channel::isMember(client* member) const
{
    if (this->members.count(member))
        return true;
    return false;
}

int Channel::getLimit() const
{
    return this->limit;
}

bool Channel::isKeySet() const
{
    return this->is_key_set;
}

std::string Channel::getKey() const
{
    return this->key;
}
void Channel::brodcastMsg(std::string msg, client *sender) const
{
    // std::string c_msg = sender->getNick() + ": " + msg + "\n";
    for(std::set<client *>::iterator it = members.begin(); it != members.end(); it++)
    {
        if(*it != sender)
            sendMsg((*it)->getFd(), msg);
    }
}
