#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "client.hpp"
class channel
{
    private:
        std::string channel_pass;
        std::string channel_name;
        std::map<std::string, client*> members;
        int userlimits;
        std::map<std::string, bool> is_oper;
        std::set<std::string> invite_members;

        bool invite_only;
        bool passOk;
        std::string topic;
    public:
        bool getpassOK();
        void setpass(const std::string &pass);
        channel(std::string &name);
        const std::string getpass();
        bool get_invite_onlyOK();
        void set_invite_only(bool invite_onyl);


};
#endif