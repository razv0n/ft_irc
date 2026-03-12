#include "../includes/server.hpp"

void Server::handlePing(int client_fd, const std::vector<std::string> &tokens)
{
    if (tokens.size() < 2)
    {
        std::string msg = "PING :Not enough parameters\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    std::string msg = "ircserv : PONG " + tokens[1] + "\r\n";
    send(client_fd, msg.c_str(), msg.length(), 0);
}