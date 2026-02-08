#include "ft_irc.hpp"

int main(int ac, char **av)
{
    if(ac != 3)
    {
        std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
        return 1;
    }

    std::string port = av[1];
    std::string password = av[2];

    // i need to check if the port is valid after don't forget

    if(password.empty())
    {
        std::cerr << "Password cannot be empty" << std::endl;
        return 1;
    }

    int port_num = std::atoi(port.c_str());

    try{
        Server server(port_num, password);
        server.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}