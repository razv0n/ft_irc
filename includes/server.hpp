#ifndef SERVER_HPP
#define SERVER_HPP

#include "ft_irc.hpp"
#include <vector>
#include <map>

class client;

class Server
{
<<<<<<< HEAD
	private:
		int server_fd;
		int port;
		std::string password;
		std::vector<struct pollfd> poll_fds;
		std::map<int, client*> clientsFds;
		std::map<std::string, client*> clientsName;
	public:
		Server(int port, const std::string& password);
		~Server();
		void run();
=======
private:
	int server_fd;
	int port;
	std::string password;
	std::vector<struct pollfd> poll_fds;
	std::map<int, client *> clients;
>>>>>>> 91229dcc406c3d2a5067dd25a1ee25878db1c423

public:
	Server(int port, const std::string &password);
	~Server();
	void run();

	void handleCommand(int client_fd, const std::string &command);
	void handlePass(int client_fd, const std::vector<std::string> &tokens);
	void handleNick(int client_fd, const std::vector<std::string> &tokens);
	void handleUser(int client_fd, const std::vector<std::string> &tokens);
	void handlePing(int client_fd, const std::vector<std::string> &tokens);
	void handleQuit(int client_fd, const std::string &command);
	void removeClient(int client_fd);
	std::vector<std::string> splitCommand(const std::string &cmd);
};

#endif
