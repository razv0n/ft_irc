#include <iostream>
// #include <>
#include <arpa/inet.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;
int main()
{
    int clientsocket = socket(AF_INET, SOCK_STREAM, 0);
     struct sockaddr_in server{}; 
     
     server.sin_family = AF_INET; // version IP wich is mean v4
     server.sin_port = htons(1234); // for right reading the port for big endien and little endien we can use any port that big than 1024
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);
    connect(clientsocket, reinterpret_cast<sockaddr*>(&server),sizeof(server));
    std::cout << "connected successfuly!" << std::endl;
    close(clientsocket);
}