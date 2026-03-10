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
	void handlePrivmsg(int client_fd, const std::vector<std::string> &tokens);

	void removeClient(int client_fd);
	std::vector<std::string> splitCommand(const std::string &cmd);
};

#endif
