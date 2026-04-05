
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
    if (listen(server_fd, SOMAXCONN) == -1)
    {
        throw std::runtime_error("listen failed");
    }

    struct pollfd server_pollfd;
    server_pollfd.fd = server_fd;
    server_pollfd.events = POLLIN;
    server_pollfd.revents = 0;
    poll_fds.push_back(server_pollfd);
    while (!g_server_shutdown)
    {
        int poll_count = poll(&poll_fds[0], poll_fds.size(), -1);
        if (poll_count == -1)
        {
            if (g_server_shutdown)
                break;
            throw std::runtime_error("poll failed");    
        }
        i_poll = 0;
        for (size_t &i = i_poll; i < poll_fds.size(); ++i)
        {
            if(poll_fds[i].revents & (POLLHUP | POLLERR | POLLNVAL))
            {
                removeClient(poll_fds[i].fd);
                continue;
            }
            else if (poll_fds[i].revents & POLLIN)
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
                        removeClient(client_fd);
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
    {
        delete it->second;
    }
    clientsFds.clear();
    for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        delete it->second;
    }
    channels.clear();
    if (server_fd != -1)
    {
        close(server_fd);
        server_fd = -1;
    }
}

std::vector<std::string> Server::splitCommand(const std::string &cmd)
{
    std::vector<std::string> tokens;
    std::string last_token = "";
    std::string cmd_token = cmd;
    size_t find = cmd.find(":");

    if (find != std::string::npos)
    {
        last_token = cmd.substr(find + 1);
        cmd_token = cmd.substr(0, find);
    }
    std::istringstream iss(cmd_token);
    std::string token;
    while (iss >> token)
        tokens.push_back(token);
    if (!last_token.empty())
        tokens.push_back(last_token);
    return tokens;
}

void Server::handleCommand(int client_fd, const std::string &command)
{
    std::vector<std::string> tokens = splitCommand(command);
    if (tokens.empty())
        return;

    std::string cmd = tokens[0];

    try
    {
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
        else if (cmd == "PART")
            handlePart(client_fd, tokens);
        else if (cmd == "KICK")
            handleKick(client_fd, tokens);
        else if (cmd == "INVITE")
            handleInvite(client_fd, tokens);
        else if (cmd == "TOPIC")
            handleTopic(client_fd, tokens);
        else if (cmd == "MODE")
            handleMode(client_fd, tokens);
        else if(cmd == "CAP")
            return;
        else
        {
            std::string msg = "Unknown command : " + cmd + "\r\n";
            if(send(client_fd, msg.c_str(), msg.length(), MSG_NOSIGNAL) == -1)
            {
                if(errno == EPIPE || errno == ECONNRESET)
                    removeClient(client_fd);
            }
        }
    }
    catch (const std::exception &e)
    {
        if(*e.what() == '\0')
            removeClient(client_fd);
        else
            sendMsg(client_fd, e.what());
    }
}
