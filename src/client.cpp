/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mowardan <mowardan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 09:23:18 by mowardan          #+#    #+#             */
/*   Updated: 2026/02/12 09:23:21 by mowardan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

client::client(int fd) : client_fd(fd)
{
}

client::~client()
{
    if (client_fd != -1)
        close(client_fd);
}
