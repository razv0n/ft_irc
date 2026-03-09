/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfahmi <mfahmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 09:23:10 by mowardan          #+#    #+#             */
/*   Updated: 2026/03/09 08:22:36 by mfahmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"
#include <sstream>
#include <iostream>
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
    {
        if (token[0] == ':')
        {
            std::string remove = token.substr(1);
            std::string rest;
            if (std::getline(iss, rest))
                remove += rest;
            tokens.push_back(remove);
            break;
        }
        tokens.push_back(token);
<<<<<<< HEAD
    }
=======
    tokens.push_back(last_token);
>>>>>>> e1d27b2bb160fc632fefa376f739c340b8ff533d
    return tokens;
}

void Server::handleCommand(int client_fd, const std::string &command)
{
    std::vector<std::string> tokens = splitCommand(command);
    if (tokens.empty())
        return;

    std::string cmd = tokens[0];

    if (cmd == "PING")
        handlePing(client_fd, tokens);
    else if (cmd == "QUIT") {
        handleQuit(client_fd, command);
    }
    else if (cmd == "PASS")
        handlePass(client_fd, tokens);
    else if (cmd == "NICK")
        handleNick(client_fd, tokens);
    else if (cmd == "USER")
        handleUser(client_fd, tokens);
    else
    {
        std::string msg = "Unknown command : " + cmd + "\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
    }
}

// TODO split the cmd to the folders and structure the data 
// TODO handle the user in the right way (ask some info)  check the ping pong and why it is exist
// TODO how join start with it and why that is 
// TODO start on the join and room cmd
// TODO add some debug on the server 7l9 3lih
void Server::handlePass(int client_fd, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 2)
    {
        std::string msg = "Usage: PASS <password>\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if (clientsFds[client_fd]->getPassOk())
    {
        std::string msg = "You are already registered\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if (tokens[1] != password)
    {
        std::string msg = "Password is incorrect\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    clientsFds[client_fd]->setPassOk(true);
    send(client_fd,"the Password is correct\n",20,0);
}

void Server::handleNick(int client_fd, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 2)
    {
        std::string msg = "Usage: NICK <nickname>\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if (clientsFds[client_fd]->getNickOk())
    {
        std::string msg = "You are already registered\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if (clientsFds[client_fd]->getPassOk() == false)
    {
        std::string msg = "You are not registered\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if (clientsName.count(tokens[1]))
    {
        send(client_fd, "This nickname is already use\n", 30, 0);
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
    std::cout << "[DEBUG] handleUser called, fd=" << client_fd << ", tokens.size()=" << tokens.size() << std::endl;
    for (size_t i = 0; i < tokens.size(); ++i)
        std::cout << "  [" << i << "]=" << tokens[i] << std::endl;

    if (tokens.size() != 5)
    {
        std::string msg = "Usage: USER <username> <hostname> <servername> :<realname>\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        std::cout << "[DEBUG] USER rejected: wrong number of parameters" << std::endl;
        return;
    }
    if (clientsFds[client_fd]->getUserOk())
    {
        std::string msg = "You are already registered\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    if (clientsFds[client_fd]->getNickOk() == false)
    {
        std::string msg = "You are not registered\n";
        send(client_fd, msg.c_str(), msg.length(), 0);
        return;
    }
    clientsFds[client_fd]->setUsername(tokens[1]);
    clientsFds[client_fd]->setRealname(tokens[4]);
    clientsFds[client_fd]->setUserOk(true); // we dont need to check this USEROK mean the setregistred is ok :>
    clientsFds[client_fd]->setRegistered(true);
    std::string msg = "Welcome " + clientsFds[client_fd]->getNick() + "!\n";
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

void Server::removeClient(int client_fd)
{
    std::string nick = clientsFds[client_fd]->getNick();
    std::cout << "Client " << nick << " disconnected (fd=" << client_fd << ")" << std::endl;
    delete clientsFds[client_fd];
    clientsFds.erase(client_fd);
    clientsName.erase(nick);
    for (size_t i = 0; i < poll_fds.size(); ++i)
    {
        if (poll_fds[i].fd == client_fd)
        {
            poll_fds.erase(poll_fds.begin() + i);
            break;
        }
    }
}
