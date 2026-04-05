
#include "../includes/server.hpp"

void Server::handleQuit(int client_fd, const std::string &command) {
  std::string quit_msg = "Client quit";
  size_t pos = command.find(':');
  if (pos != std::string::npos)
    quit_msg = command.substr(pos + 1);
  removeClient(client_fd);
}

void Server::removeClient(int client_fd) 
{
  std::string nick = clientsFds[client_fd]->getNick();
  std::map<std::string, Channel *>::iterator it = channels.begin();
  std::map<std::string, Channel *>::iterator next_it;
  while (it != channels.end())
  {
    if ((it->second)->isMember(clientsFds[client_fd]))
      (it->second)->removeClient(clientsFds[client_fd]);
    if ((it->second)->getInvites().count(clientsFds[client_fd]))
      (it->second)->removeInvite(clientsFds[client_fd]);
    if ((it->second)->getOperators().count(clientsFds[client_fd]))
      (it->second)->removeOperator(clientsFds[client_fd]);
    if ((it->second)->channelEmpty())
    {
      next_it = it;
      next_it++;
      std::string _name = it->second->getName();
      delete it->second;
      channels.erase(_name);
      it = next_it;
    } 
    else
      it++;
  }
  delete clientsFds[client_fd];
  clientsFds.erase(client_fd);
  clientsName.erase(nick);
  for (size_t i = 0; i < poll_fds.size(); ++i) {
    if (poll_fds[i].fd == client_fd) {
      poll_fds.erase(poll_fds.begin() + i);
      if(i_poll > 0)
        i_poll--;
      break;
    }
  }
}