
#include "../includes/server.hpp"

void Server::handleQuit(int client_fd, const std::string &command)
{
    // Parse quit message: everything after "QUIT" and optional ":"
    std::string quit_msg = "Client quit";
    size_t pos = command.find(':');
    if (pos != std::string::npos)
        quit_msg = command.substr(pos + 1);

    // TODO: later when channels are implemented,
    // notify all channel members about the quit

    removeClient(client_fd);
}

void Server::removeClient(int client_fd)
{
    std::string nick = clientsFds[client_fd]->getNick();
    // std::cout << "Client " << nick << " disconnected (fd=" << client_fd << ")" << std::endl;
    delete clientsFds[client_fd];
    clientsFds.erase(client_fd);
    clientsName.erase(nick);
    for(std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if((it->second)->isMember(clientsFds[client_fd]))
            (it->second)->removeClient(clientsFds[client_fd]);
        if((it->second)->getInvites().count(clientsFds[client_fd]))
            (it->second)->removeInvite(clientsFds[client_fd]);
        if((it->second)->getOperators().count(clientsFds[client_fd]))
            (it->second)->removeOperator(clientsFds[client_fd]);
        if((it->second)->channelEmpty())
        {
            delete channels[it->second->getName()];
            channels.erase(it->second->getName());
        }
    }
    for (size_t i = 0; i < poll_fds.size(); ++i)
    {
        if (poll_fds[i].fd == client_fd)
        {
            poll_fds.erase(poll_fds.begin() + i);
            break;
        }
    }
}