
#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "client.hpp"
#include "ft_irc.hpp"
#include <vector>

class client;

class Channel {
private:
    // Amine and monsif need to see this;
    std::string name; // #channel
    std::string topic; // topic of the channel
    std::set<client *> members; // clients in the channel
    std::set<client *> operators; // operators in the channel --> @admine == the first who create the channel
    std::set<client *> invites; // invited clients in the channel --> MODE +i
    bool is_invite_only; // MODE +i --> responsible for make channel add just invited users
    bool is_topic_set; // boolean to see if the channel has a topic
    bool is_topic_protected; // MODE +t --> responsible for make topic only operators can set it
    bool is_limit_set; // boolean to see if the channel has a limit
    int limit; // MODE +l --> responsible for make a limit/size for channel
    int currentLimit;
    bool is_key_set; // boolean to see if the channel has a key
    std::string key; // MODE +k --> responsible for make a key for channel
public:
    Channel(const std::string& name, client* creator);
    ~Channel();

    void addClient(client* client);
    void removeClient(client* client);
    void addOperator(client* client);
    bool channelEmpty();
    void incLimit();
    void removeOperator(client* client);
    void addInvite(client* client);
    void removeInvite(client* client);
    int getCurrLimit();
    void setTopic(const std::string& topic);
    bool isOperator(client* member);
    void setKey(const std::string& key);
    void setLimit(int limit);
    void setInviteOnly(bool inviteOnly); // MODE +i
    void setTopicProtected(bool topicProtected); // MODE +t
    void setLimitSet(bool limitSet); // MODE +l
    void setKeySet(bool keySet); // MODE +k
    std::string getName() const;
    std::string getTopic() const;
    std::set<client*> getClients() const;
    std::set<client*> getOperators() const;
    std::set<client*> getInvites() const;
    bool isInviteOnly() const;
    bool isTopicSet() const;
    bool isTopicProtected() const;
    bool isLimitSet() const;
    void sendMsg(int member_fd, std::string msg);
    bool isMember(client* client) const;
    int getLimit() const;
    bool isKeySet() const;
    std::string getKey() const;
    void brodcastMsg(std::string msg, client *sender) const;
};

#endif