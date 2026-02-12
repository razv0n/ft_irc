/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mowardan <mowardan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 09:23:10 by mowardan          #+#    #+#             */
/*   Updated: 2026/02/12 09:23:12 by mowardan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"


Server::Server(int port, const std::string& password)
{
    server_fd = -1;
    this->port = port;
    this->password = password;
    
}


void Server::run()
{
    // TODO: implement the server loop
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == -1) 
    {
        throw std::runtime_error("socket creation failed");
    }
    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    if(bind(server_fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == -1) 
    {
        throw std::runtime_error("bind failed");
    }
    if(listen(server_fd, 128) == -1) 
    {
        throw std::runtime_error("listen failed");
    }
    // Allow reuse of address/port (prevents "Address already in use" error)
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct pollfd server_pollfd;
    server_pollfd.fd = server_fd;
    server_pollfd.events = POLLIN;
    server_pollfd.revents = 0;
    poll_fds.push_back(server_pollfd);
    while(true)
    {
        // TODO: use poll to handle multiple clients
        int poll_count = poll(&poll_fds[0], poll_fds.size(), -1);
        if(poll_count == -1) 
        {
            throw std::runtime_error("poll failed");
        }
        for(size_t i = 0; i < poll_fds.size(); ++i) 
        {
            if(poll_fds[i].revents & POLLIN)
            {
                if(poll_fds[i].fd == server_fd) 
                {
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client_fd = accept(server_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_len);
                    if(client_fd == -1)
                    {
                        std::cerr << "accept failed" << std::endl;
                        continue;
                    }
                    struct pollfd client_pollfd;
                    client_pollfd.fd = client_fd;
                    client_pollfd.events = POLLIN;
                    poll_fds.push_back(client_pollfd);
                    clients[client_fd] = new client(client_fd);
                } 
                else 
                {
                    int client_fd = poll_fds[i].fd;
                    char buffer[1024];
                    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
                    if(bytes_received <= 0) 
                    {
                        close(client_fd);
                        delete clients[client_fd];
                        clients.erase(client_fd);
                        poll_fds.erase(poll_fds.begin() + i);
                        --i;
                    } 
                    else 
                    {
                        std::string data(buffer, bytes_received);
                        std::cout << "Received from client " << client_fd << ": " << data ;
                    }
                }
            }
        }
    }
}

Server::~Server()
{
    if (server_fd != -1)
        close(server_fd);
}


