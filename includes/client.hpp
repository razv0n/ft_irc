#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class client
{
private:
    int client_fd;
    std::string nickname;
    std::string username;
    std::string realname;

public:
    client(int fd);
    ~client();
};

#endif