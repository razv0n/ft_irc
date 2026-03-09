/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfahmi <mfahmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 09:23:27 by mowardan          #+#    #+#             */
/*   Updated: 2026/03/08 10:23:04 by mfahmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/ft_irc.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
        return 1;
    }

    std::string port = av[1];
    std::string password = av[2];
    if (port.empty())
    {
        std::cerr << "Port cannot be empty" << std::endl;
        return 1;
    }
    for (size_t i = 0; i < port.length(); i++)
    {
        if (port[i] < '0' || port[i] > '9')
        {
            std::cerr << "Port must contain only digits" << std::endl;
            return 1;
        }
    }
    int port_num = std::atoi(port.c_str());
    if (port_num < 1024 || port_num > 65535)
    {
        std::cerr << "Port must be between 1024 and 65535" << std::endl;
        return 1;
    }
    else if (password.empty())
    {
        std::cerr << "Password cannot be empty" << std::endl;
        return 1;
    }
    try
    {
        Server server(port_num, password);
        server.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
