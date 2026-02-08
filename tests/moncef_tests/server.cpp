#include <iostream>
// #include <>
#include <arpa/inet.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string> 
using namespace std;
int main()
{
    int serversocket = socket(AF_INET, SOCK_STREAM, 0);
     struct sockaddr_in server{}; 
     
     server.sin_family = AF_INET; // version IP wich is mean v4
     server.sin_port = htons(1234); // for right reading the port for big endien and little endien we can use any port that big than 1024 less than it u should have root access LLLLOOLL //!binding socket
    server.sin_addr.s_addr = INADDR_ANY;
    bind(serversocket, reinterpret_cast<sockaddr *>(&server), sizeof(server)); // binding the socket with the ip and port
    // bind we go for listen
    listen(serversocket, 5); // start to listeing to tha port in that ip address so it store to the quenne for waiting if the quenne is allready full the client get refused //!(passive socket)
    struct sockaddr_in  client{}; 
    socklen_t client_len = sizeof(client);
    int clientsocket = accept(serversocket, reinterpret_cast<sockaddr*>(&client), &client_len);
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client.sin_addr, client_ip, sizeof(client_ip));
    cout << "Client connected: " << client_ip
     << ":" << ntohs(client.sin_port) << endl;
     char buffer[1000];
     recv(clientsocket, buffer, sizeof(buffer), 0);
     cout << "Received from the client :" << buffer << endl;
    char msg[] = "Hello from server!";
    send(clientsocket, msg, sizeof(msg), 0);
}

