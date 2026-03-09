/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfahmi <mfahmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 09:23:18 by mowardan          #+#    #+#             */
/*   Updated: 2026/03/08 08:55:58 by mfahmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/client.hpp"

client::client(int fd) : client_fd(fd), is_registered(false), pass_ok(false), nick_ok(false), user_ok(false) {

}

client::~client() {
  if (client_fd != -1)
    close(client_fd);
}

void client::appendToBuffer(const std::string &data) {
    buffer += data;
    if(*(buffer.end() - 1) == '\n')
        std::cout << "client number : " << client_fd << " the msg is : " << buffer;
}

std::string client::extractCommand() {
  size_t pos = buffer.find("\n");
  if (pos == std::string::npos)
    return "";
  
  std::string cmd = buffer.substr(0, pos + 1);
  buffer.erase(0, pos + 1);

  if (cmd.length() > 1 && cmd[cmd.length() - 2] == '\r')
    cmd.erase(cmd.length() - 2, 2);
  else if (cmd.length() > 0 && cmd[cmd.length() - 1] == '\n')
    cmd.erase(cmd.length() - 1, 1);

  return cmd;
}

bool client::getPassOk() const { 
    return pass_ok; 
}

void client::setPassOk(bool status) { 
    pass_ok = status; 
}

bool client::getNickOk() const {
    return nick_ok; 
}

void client::setNickOk(bool status) { 
    nick_ok = status;
}

bool client::getUserOk() const { 
    return user_ok; 
}

void client::setUserOk(bool status) {
    user_ok = status; 
}

bool client::isRegistered() const {
    return is_registered; 
}

void client::setRegistered(bool status) {
    is_registered = status; 
}

void client::setNick(const std::string& nick) {
    nickname = nick;
}
std::string client::getNick() const {
    return nickname;
}
void client::setUsername(const std::string& username) {
    this->username = username;
}
std::string client::getUsername() const {
    return username;
}
void client::setRealname(const std::string& realname) {
    this->realname = realname;
}
std::string client::getRealname() const {
    return realname;
}
