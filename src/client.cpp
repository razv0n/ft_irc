#include "client.hpp"

client::client(int fd) : client_fd(fd)
{
}

client::~client()
{
    if (client_fd != -1)
        close(client_fd);
}
