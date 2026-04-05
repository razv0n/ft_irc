#ifndef SERVER_HPP
#define SERVER_HPP

#include "ft_irc.hpp"
#include <vector>
#include <map>
#include "Channel.hpp"

class client;
class Channel;

class Server
{
	private:
		int server_fd;
		int port;
		std::string password;
		size_t i_poll;
		std::vector<struct pollfd> poll_fds;
		std::map<int, client*> clientsFds;
		std::map<std::string, client*> clientsName;
		std::map<std::string, Channel*> channels;
	public:
		Server(int port, const std::string& password);
		~Server();
		void run();


	void handleCommand(int client_fd, const std::string &command);
	void handlePass(int client_fd, const std::vector<std::string> &tokens);
	void handleNick(int client_fd, const std::vector<std::string> &tokens);
	void handleUser(int client_fd, const std::vector<std::string> &tokens);
	void handlePing(int client_fd, const std::vector<std::string> &tokens);
	void handleQuit(int client_fd, const std::string &command);
	void handleJoin(int client_fd, const std::vector<std::string> &tokens);
	void handlePart(int client_fd, const std::vector<std::string> &tokens);
	void handlePrivmsg(int client_fd, const std::vector<std::string> &tokens);
	void handleKick(int client_fd, const std::vector<std::string> &tokens);
	void handleInvite(int client_fd, const std::vector<std::string> &tokens);
	void handleTopic(int client_fd, const std::vector<std::string> &tokens);
	void removeClient(int client_fd);
	void handleMode(int client_fd, const std::vector<std::string> &tokens);
	// void handleWho(int client_fd, const std::vector<std::string> &tokens);

	void checkChannelName(std::string channel_name, std::string member_name);
void checkIsMember(std::string channel_name, client *member, std::string member_name);
void checkIsOperator(std::string channel_name, client *member);
void checkIsInvite(std::string , client *);
void checkChannelExist(std::string channel_name,std::string member_name);
void isRegistered(client *member);
void checkClientExist(std::string target_name, std::string requester_name, std::string channel_name);
	std::vector<std::string> splitCommand(const std::string &cmd);
};

#endif
