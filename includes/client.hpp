#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "ft_irc.hpp"

class client
{
	private:
		int client_fd;
		std::string nickname;
		std::string username;
		std::string realname;
		std::string buffer;
		bool is_registered;
		bool pass_ok;
		bool nick_ok;
		bool user_ok;
	public:
		client(int fd);
		~client();

		void appendToBuffer(const std::string& data);
		std::string extractCommand();

		bool getPassOk() const;
		void setPassOk(bool status);
		bool getNickOk() const;
		void setNickOk(bool status);
		int getFd() const;
		bool getUserOk() const;
		void setUserOk(bool status);
		bool isRegistered() const;
		void setRegistered(bool status);
		void setNick(const std::string& nick);
		std::string getNick() const;
		void setUsername(const std::string& username);
		std::string getUsername() const;
		void setRealname(const std::string& realname);
		std::string getRealname() const;
};

#endif
