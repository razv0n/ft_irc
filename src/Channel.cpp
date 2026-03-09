#include "../include/Channel.hpp"

Channel::Channel(const std::string& name, client* creator)
{
    this->name = name;
    this->topic = "";
    this->clients.push_back(creator);
    this->operators.push_back(creator);
    this->invites.clear(); // the purpose of this is to make sure that the channel is not invite only by default
    this->is_invite_only = false;
    this->is_topic_set = false;
    this->is_topic_protected = false;
    this->is_limit_set = false;
    this->limit = 0;
    this->is_key_set = false;
    this->key = "";
    this->is_anonymous = false;
}

Channel::~Channel()
{
    this->clients.clear();
    this->operators.clear();
    this->invites.clear();
}

void Channel::addClient(client* client)
{
    if (!isMember(client))
        this->clients.push_back(client);
}

void Channel::removeClient(client* client)
{
    for (size_t i = 0; i < this->clients.size(); i++)
    {
        if (this->clients[i] == client) 
        {
            this->clients.erase(this->clients.begin() + i);
            break;
        }
    }
}

void Channel::addOperator(client* client)
{
    this->operators.push_back(client);
}

void Channel::removeOperator(client* client)
{
    for (size_t i = 0; i < this->operators.size(); i++)
    {
        if (this->operators[i] == client)
        {
            this->operators.erase(this->operators.begin() + i);
            break;
        }
    }
}

void Channel::addInvite(client* client)
{
    this->invites.push_back(client);
}

void Channel::removeInvite(client* client)
{
    for (size_t i = 0; i < this->invites.size(); i++)
    {
        if (this->invites[i] == client)
        {
            this->invites.erase(this->invites.begin() + i);
            break;
        }
    }
}

void Channel::setTopic(const std::string& topic)
{
    this->topic = topic;
    this->is_topic_set = true;
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

std::vector<client*> Channel::getClients() const
{
    return this->clients;
}

std::vector<client*> Channel::getOperators() const
{
    return this->operators;
}

std::vector<client*> Channel::getInvites() const
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

bool Channel::isMember(client* client) const
{
    for (size_t i = 0; i < this->clients.size(); i++)
    {
        if (this->clients[i] == client)
            return true;
    }
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