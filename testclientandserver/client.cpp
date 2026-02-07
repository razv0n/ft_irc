// 


#include <string>
#include <cstring>
// #include <socket.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

using namespace std;

int main() {
    int client;
    int buffer_size = 1024;
    int port_number = 1500; // note : this is the port number that the server will listen to the client . it should be the same as the client port number.
    char buffer[buffer_size];
    struct sockaddr_in server_addr;
    const char *ip = "127.0.0.1";

   // init socket 
   client = socket(AF_INET, SOCK_STREAM, 0);
    if(client < 0)
    {
        cerr << "ERROR : socket creation failed" << endl;
        return 1;
    }
    cout << "socket creation successful ......... " << endl;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_number);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if(connect(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        cerr << "ERROR : socket connection failed" << endl;
        return 1;
    }
    cout << "socket connection successful ......... " << endl;

    cout << "client : ";
    memset(buffer, 0, buffer_size);
    cin.getline(buffer, buffer_size);
    while(strcmp(buffer, "exit") != 0)
    {
        send(client, buffer, strlen(buffer), 0);
        memset(buffer, 0, buffer_size);
        recv(client, buffer, buffer_size, 0);
        cout << "server : " << buffer << endl;
        cout << "client : ";
        memset(buffer, 0, buffer_size);
        cin.getline(buffer, buffer_size);
    }
    cout << "connection closed ... " << endl;
    close(client);
    return 0;
}