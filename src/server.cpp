
#include "../includes/server.hpp"
#include "../includes/client.hpp"

Server::Server(int port, const std::string &password)
{
    server_fd = -1;
    this->port = port;
    this->password = password;
}

void Server::run()
{
    // TODO: implement the server loop

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        throw std::runtime_error("socket creation failed");
    }
    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (bind(server_fd, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr)) == -1)
    {
        throw std::runtime_error("bind failed");
    }
    if (listen(server_fd, 128) == -1)
    {
        throw std::runtime_error("listen failed");
    }

    struct pollfd server_pollfd;
    server_pollfd.fd = server_fd;
    server_pollfd.events = POLLIN;
    server_pollfd.revents = 0;
    poll_fds.push_back(server_pollfd);
    while (true)
    {
        int poll_count = poll(&poll_fds[0], poll_fds.size(), -1);
        if (poll_count == -1)
        {
            throw std::runtime_error("poll failed");
        }
        for (size_t i = 0; i < poll_fds.size(); ++i)
        {
            if (poll_fds[i].revents & POLLIN)
            {
                if (poll_fds[i].fd == server_fd)
                {
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client_fd = accept(server_fd, reinterpret_cast<sockaddr *>(&client_addr), &client_len);
                    if (client_fd == -1)
                    {
                        std::cerr << "accept failed" << std::endl;
                        continue;
                    }
                    struct pollfd client_pollfd;
                    client_pollfd.fd = client_fd;
                    client_pollfd.events = POLLIN;
                    client_pollfd.revents = 0;
                    poll_fds.push_back(client_pollfd);
                    clientsFds[client_fd] = new client(client_fd);
                }
                else
                {
                    int client_fd = poll_fds[i].fd;
                    char buffer[1024];
                    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
                    if (bytes_received <= 0)
                    {
                        close(client_fd);
                        delete clientsFds[client_fd];
                        clientsFds.erase(client_fd);
                        poll_fds.erase(poll_fds.begin() + i);
                        --i;
                    }
                    else
                    {
                        std::string data(buffer, bytes_received);
                        clientsFds[client_fd]->appendToBuffer(data);

                        std::string cmd;
                        while ((cmd = clientsFds[client_fd]->extractCommand()) != "")
                        {
                            handleCommand(client_fd, cmd);
                            if (clientsFds.find(client_fd) == clientsFds.end())
                                break;
                        }
                    }
                }
            }
        }
    }
}

Server::~Server()
{
    for (std::map<int, client *>::iterator it = clientsFds.begin(); it != clientsFds.end(); ++it)
        delete it->second;
    clientsFds.clear();
    if (server_fd != -1)
        close(server_fd);
}

std::vector<std::string> Server::splitCommand(const std::string &cmd)
{
    std::vector<std::string> tokens;
    std::string last_token = "";
    std::string cmd_token = cmd;
    size_t find = cmd.find(":");

    if(find != std::string::npos)
    {
        last_token = cmd.substr(find + 1);
        cmd_token = cmd.substr(0, find);
    }
    std::istringstream iss(cmd_token);
    std::string token;
    while (iss >> token)
        tokens.push_back(token);
    if(!last_token.empty())
        tokens.push_back(last_token);
    return tokens;
}

void Server::handleCommand(int client_fd, const std::string &command)
{
    std::vector<std::string> tokens = splitCommand(command);
    if (tokens.empty())
        return;

    std::string cmd = tokens[0];

    try{
    
        if (cmd == "PING")
            handlePing(client_fd, tokens);
        else if (cmd == "QUIT")
            handleQuit(client_fd, command);
        else if (cmd == "PASS")
            handlePass(client_fd, tokens);
        else if (cmd == "NICK")
            handleNick(client_fd, tokens);
        else if (cmd == "USER")
            handleUser(client_fd, tokens);
        else if (cmd == "JOIN")
            handleJoin(client_fd, tokens);
        else if (cmd == "PRIVMSG")
            handlePrivmsg(client_fd, tokens);
        else if(cmd == "PART")
            handlePart(client_fd, tokens);
        else if(cmd == "KICK")
            handleKick(client_fd, tokens);
        else if(cmd == "INVITE")
            handleInvite(client_fd, tokens);
        else if(cmd == "TOPIC")
            handleTopic(client_fd, tokens);
        else if(cmd == "MODE")
            handleMode(client_fd, tokens);
        else
        {
            std::string msg = "Unknown command : " + cmd + "\r\n";
            send(client_fd, msg.c_str(), msg.length(), 0);
        }
    }
    catch(...)
    {
        
    }
}

// TODO split the cmd to the folders and structure the data 
// TODO handle the user in the right way (ask some info)  check the ping pong and why it is exist
// TODO add some debug on the server 7l9 3lih
//TODO every msg should be end with \r\n
// TODO test the code in irssi client
//TODO split the cmd functions
// TODO check the overflow of the long nm
int isValidNm(std::string strNm)
{
    char *end;
    long nm = strtol(strNm.c_str(), &end, 10);
    if(*end != '\0' || nm <= 0 || nm > INT_MAX || nm < INT_MIN)
        return 0;
    return nm;
}
void Server::handleMode(int client_fd, const std::vector<std::string> &tokens)
{
    if(!clientsFds[client_fd]->isRegistered())
    {
        std::string msg = "You are not registered\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(tokens.size() < 3 || tokens.size() > 4)
    {
        std::string msg = "Usage: MODE <#channel> +/-mode [parameter]\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    std::string channel_name = tokens[1];
    std::string mode = tokens[2];
    if(channel_name[0] != '#')
    {
        std::string msg = "Invalid channel name\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(!channels[channel_name]->isMember(clientsFds[client_fd]))
    {
        std::string msg = "you are not a member inside this channel\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(!channels[channel_name]->isOperator(clientsFds[client_fd]))
    {
        std::string msg = "you are not an operator inside this channel\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(mode.size() != 3 || mode[0] != '-' || mode[0] != '+')
    {
        std::string msg = "the mode is incorrect\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    switch (mode[1])
    {
    case 'i':
        channels[channel_name]->setInviteOnly(mode[0] == '-' ? false: true);
        return;
    case 't':
        channels[channel_name]->setTopicProtected(mode[0] == '-' ? false: true);
        return;
    case 'k':
        if(tokens.size() == 4 && mode[0] == '+')
        {
            std::string new_key = tokens[3];
            channels[channel_name]->setKey(new_key);
            channels[channel_name]->setKeySet(true);
            return;
        }
        else if(tokens.size() == 3 && mode[0] == '-')
        {
            channels[channel_name]->setKey("");
            channels[channel_name]->setKeySet(false);
            return;
        }
        break;
    case 'o':
        if(!channels[channel_name]->isMember(clientsName[tokens[3]]))
        {
            std::string msg = "the " + tokens[3] + " is not a member inside this channel \r\n";
            send(client_fd, msg.c_str(), msg.length(), 0);
            return;
        }
        if(tokens.size() == 4)
        {
            if(mode[0] == '-')
                channels[channel_name]->removeOperator(clientsName[tokens[3]]);
            else
                channels[channel_name]->addOperator(clientsName[tokens[3]]);
            return;
        }
        break;
    case 'l':
        if(tokens.size() == 4 && mode[0] == '+')
        {
            std::string new_limit = tokens[3];
            if(int limit = isValidNm(new_limit))
            {
                channels[channel_name]->setLimit(limit); 
                channels[channel_name]->setLimitSet(true);
                return;
            }
            else
            {
                std::string msg = "the number is incorrect\r\n";
                send(client_fd, msg.c_str(), msg.length(), 0);
                return;         
            }
        }
        else if(tokens.size() == 3 && mode[0] == '-')
        {
            channels[channel_name]->setLimit(0);
            channels[channel_name]->setLimitSet(false);
            return;
        }
        break;
    }
    std::string msg = "the mode is incorrect\r\n";
    send(client_fd, msg.c_str(), msg.length(), 0);
}

void Server::handleTopic(int client_fd, const std::vector<std::string> &tokens)
{
    if(!clientsFds[client_fd]->isRegistered())
    {
        std::string msg = "You are not registered\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    size_t tokensSize = tokens.size();
    if(tokensSize > 3)
    {
        std::string msg = "Usage: TOPIC <#channel>\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    std::string channel_name = tokens[1];
    if(!channels[channel_name]->isMember(clientsFds[client_fd]))
    {
        std::string msg = "you are not a member inside this channel\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(channel_name[0] != '#')
    {
        std::string msg = "Invalid channel name\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(tokensSize == 2)
    {
        std::string topic = channels[channel_name]->isTopicSet() ?  channels[channel_name]->getTopic(): "No topic is set";
        std::string msg = "ircserv " + clientsFds[client_fd]->getNick() + " " + channel_name + " :" + topic + "\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    else if (channels[channel_name]->isTopicProtected() && !channels[channel_name]->getOperators().count(clientsFds[client_fd]))
    {
        std::string msg = "ircserv " + clientsFds[client_fd]->getNick() + " you are not an operator inside " + channel_name + "\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    std::string topic = tokens[2];
    channels[channel_name]->setTopic(topic);
}
void Server::handleInvite(int client_fd, const std::vector<std::string> &tokens)
{
    if(!clientsFds[client_fd]->isRegistered())
    {
        std::string msg = "You are not registered\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(tokens.size() > 4 || tokens.size() < 3)
    {
        send(client_fd, "Usage: INVITE <nick> <#channel>\r\n", 30, 0);
        return;
    }
    std::string channel_name = tokens[2];
    std::string invite_name = tokens[1];

    if(channel_name[0] != '#')
    {
        std::string msg = "Invalid channel name\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(!channels.count(channel_name))
    {
        std::string msg = "the channel not found\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(!channels[channel_name]->isMember(clientsFds[client_fd]))
    {
        std::string msg = "you are not a member inside this channel\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(!channels[channel_name]->isOperator(clientsFds[client_fd]))
    {
        std::string msg = "you are not an operator inside this channel\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(!channels[channel_name]->isMember(clientsName[invite_name]))
    {
        std::string msg = "the " + invite_name +"is not a member inside this channel\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    std::string msg = ":" + clientsFds[client_fd]->getNick() + " INVITE " + invite_name + " " + channel_name + "\r\n";
    send(clientsName[invite_name]->getFd(), msg.c_str(), msg.length(), 0);
    channels[channel_name]->addInvite(clientsFds[client_fd]);
}
void Server::handleKick(int client_fd, const std::vector<std::string> &tokens)
{
    if(!clientsFds[client_fd]->isRegistered())
    {
        std::string msg = "You are not registered\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(tokens.size() > 4 || tokens.size() < 3)
    {
        send(client_fd, "Usage: KICK <channel> <nick> :<msg>\r\n", 30, 0);
        return;
    }
    std::string channel_name = tokens[1];
    if(channel_name[0] != '#')
    {
        std::string msg = "Invalid channel name\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(!channels.count(channel_name))
    {
        std::string msg = "the channel not found\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(!channels[channel_name]->isMember(clientsFds[client_fd]))
    {
        std::string msg = "you are not a member inside this channel\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(!channels[channel_name]->isOperator(clientsFds[client_fd]))
    {
        std::string msg = "you are not an operator inside this channel\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    std::string kick_name = tokens[2];
    if(!channels[channel_name]->isMember(clientsName[tokens[2]]))
    {
        std::string msg = "the " + tokens[2] +"is not a member inside this channel\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(tokens[2] == clientsFds[client_fd]->getNick())
    {
        std::string msg = "really nega :>\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(channels[channel_name]->isOperator(clientsName[kick_name]))
        channels[channel_name]->removeOperator(clientsName[kick_name]);
    if(channels[channel_name]->isInviteOnly())
        channels[channel_name]->removeInvite(clientsName[kick_name]);
    channels[channel_name]->removeClient(clientsName[kick_name]);
    std::string msg = "";
    if(tokens.size() == 4)
        msg = tokens[3];
    msg = ":" + clientsFds[client_fd]->getNick() + "!"+ clientsFds[client_fd]->getUsername() + " KICK "+ channel_name + kick_name + " :" + msg +"\r\n";
    channels[channel_name]->brodcastMsg(msg, NULL);
}
void Server::handlePart(int client_fd, const std::vector<std::string> &tokens)
{
    if(!clientsFds[client_fd]->isRegistered())
   {
       std::string msg = "You are not registered\r\n";
       send(client_fd, msg.c_str(), msg.length(), 0);
       return;
   }
    if(tokens.size() > 2 || tokens.size() < 3)
    {
        send(client_fd, "Usage: PART <channel> :<msg>\r\n", 30, 0);
        return;
    }
    std::string channel_name = tokens[1];
    if(channel_name[0] != '#')
    {
        std::string msg = "Invalid channel name\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(!channels.count(channel_name))
    {
        std::string msg = "the channel not found\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(!channels[channel_name]->isMember(clientsFds[client_fd]))
    {
        std::string msg = "you are not a member inside this channel\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(channels[channel_name]->getInvites().count(clientsFds[client_fd]))
        channels[channel_name]->removeInvite(clientsFds[client_fd]);
    if(channels[channel_name]->getOperators().count(clientsFds[client_fd]))
        channels[channel_name]->removeOperator(clientsFds[client_fd]);
    channels[channel_name]->removeClient(clientsFds[client_fd]);
    if(channels[channel_name]->channelEmpty())
    {
        delete channels[channel_name];
        channels.erase(channel_name);
    }
    else if(tokens.size() == 3)
    {
        std::string msg =  ":" + clientsFds[client_fd]->getNick() + "!"+ clientsFds[client_fd]->getUsername() + " PART "+ channel_name + " :" + tokens[2] + "\r\n";
        channels[channel_name]->brodcastMsg(msg, clientsFds[client_fd]);
    }
}
void Server::handlePrivmsg(int client_fd, const std::vector<std::string> &tokens)
{
    if(!clientsFds[client_fd]->isRegistered())
    {
        std::string msg = "You are not registered\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(tokens.size() > 3 || tokens.size() < 3)
    {
        send(client_fd, "Usage: PRIVMSG <target> <text>\r\n", 32, 0);
        return;
    }
    std::string channel_name = tokens[1];
    if(channel_name[0] != '#')
    {
        std::string msg = "Invalid channel name\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(!channels.count(channel_name))
    {
        std::string msg = "the channel not found\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if(!channels[channel_name]->isMember(clientsFds[client_fd]))
    {
        std::string msg = "you are not a member inside this channel\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    channels[channel_name]->brodcastMsg(clientsFds[client_fd]->getNick() + " : " + tokens[2] + "\r\n", clientsFds[client_fd]);
    
}

void Server::handlePass(int client_fd, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 2)
    {
        std::string msg = "Usage: PASS <password>\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if (clientsFds[client_fd]->getPassOk())
    {
        std::string msg = "You are already registered\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if (tokens[1] != password)
    {
        std::string msg = "Password is incorrect\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    clientsFds[client_fd]->setPassOk(true);
    send(client_fd,"the Password is correct\r\n",25,0);
}

void Server::handleNick(int client_fd, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 2)
    {
        std::string msg = "Usage: NICK <nickname>\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if (clientsFds[client_fd]->getPassOk() == false)
    {
        std::string msg = "You are not registered\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if (clientsName.count(tokens[1]))
    {
        send(client_fd, "This nickname is already use\r\n", 30, 0);
        return;
    }
    if(clientsFds[client_fd]->getNickOk()) // can i if i want notify channels
        clientsName.erase(clientsFds[client_fd]->getNick());
    else
        clientsFds[client_fd]->setNickOk(true);
    clientsFds[client_fd]->setNick(tokens[1]);
    clientsName[tokens[1]] = clientsFds[client_fd];
}

void Server::handleUser(int client_fd, const std::vector<std::string> &tokens)
{
    if (tokens.size() > 5 || tokens.size() < 5)
    {
        std::string msg = "Usage: USER <username> <hostname> <servername> :<realname>\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        // std::cout << "[DEBUG] USER rejected: wrong number of parameters" << std::endl;
        return;
    }
    if (clientsFds[client_fd]->getUserOk())
    {
        std::string msg = "You are already registered\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if (clientsFds[client_fd]->getNickOk() == false)
    {
        std::string msg = "You are not registered\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    clientsFds[client_fd]->setUsername(tokens[1]);
    clientsFds[client_fd]->setRealname(tokens[4]);
    clientsFds[client_fd]->setUserOk(true); // we dont need to check this USEROK mean the setregistred is ok :>
    clientsFds[client_fd]->setRegistered(true);
    std::string msg = "Welcome " + clientsFds[client_fd]->getNick() + "!\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
}

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

void Server::handleJoin(int client_fd, const std::vector<std::string> &tokens)
{
    if (clientsFds[client_fd]->isRegistered() == false)
    {
        std::string msg = "You are not registered\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if (tokens.size() < 2)
    {
        std::string msg = "Usage: JOIN <channel> [key]\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    std::string channel_name = tokens[1];
    if (channel_name[0] != '#')
    {
        std::string msg = "Invalid channel name\r\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    std::string key = "";
    if (tokens.size() > 2) {
        key = tokens[2]; 
    }
    if (channels.find(channel_name) == channels.end())
    {
        channels[channel_name] = new Channel(channel_name, clientsFds[client_fd]);
        if(!key.empty())
            channels[channel_name]->setKey(key);
    }
    else
    {
        if (channels[channel_name]->isKeySet() && channels[channel_name]->getKey() != key)
        {
            std::string msg = "Invalid key\r\n";
            send(client_fd, msg.c_str(), msg.length(), 0);
            return;
        }
        else if(channels[channel_name]->isInviteOnly() && !channels[channel_name]->getInvites().count(clientsFds[client_fd]))
        {
            std::string msg = ":ircserv " + clientsFds[client_fd]->getNick() + " " + channel_name + " :Cannot join channel (+i)\r\n";
            send(client_fd, msg.c_str(), msg.length(), 0);
            return;
        }
        if(channels[channel_name]->isLimitSet())
        {
            if(channels[channel_name]->getLimit() == channels[channel_name]->getCurrLimit())
            {
                std::string msg = ":ircserv " + clientsFds[client_fd]->getNick() + " " + channel_name + " :Cannot join channel (+l the channel is full)\r\n";
                send(client_fd, msg.c_str(), msg.length(), 0);
                return  ;
            }
            channels[channel_name]->incLimit();
        }
        channels[channel_name]->addClient(clientsFds[client_fd]);
    }
    std::string msg = "Welcome " + clientsFds[client_fd]->getNick() + "!\r\n";
    send(client_fd, msg.c_str(), msg.length(), 0);
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
