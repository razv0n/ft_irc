#include "../includes/server.hpp"

void Server::handlePing(int client_fd, const std::vector<std::string> &tokens)
{
    (void)client_fd;
    if (tokens.size() < 2)
        throw std::runtime_error("PING :Not enough parameters");
    throw std::runtime_error("ircserv : PONG " + tokens[1]);
}