// this is the server side . it will listen to the client and send back the message to the client .



#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

using namespace std;


int main() {
    int client, server;
    int buffer_size = 1024;
    int port_number = 1500;
    char buffer[buffer_size];
    struct sockaddr_in server_addr;

    socklen_t addr_size;

    // init so  cket

    client = socket(AF_INET, SOCK_STREAM, 0);
    if(client < 0)
    {
        cerr << "ERROR : socket creation failed" << endl;
        return 1;
    }
    cout << "socket creation successful ......... " << endl;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port_number);
    // biding the socket 
    if(bind(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        cerr << "ERROR : socket binding failed" << endl;
        return 1;
    }
    cout << "socket binding successful ......... " << endl;
    addr_size = sizeof(server_addr);
    
    cout << "looking for clients ......... " << endl;
    listen(client, 1);

    // accepting the client
    server = accept(client, (struct sockaddr*)&server_addr, &addr_size);
    if(server < 0)
    {
        cerr << "ERROR : socket accepting failed" << endl;
        return 1;
    }
    cout << "client accepted ......... " << endl;

    while(server > 0)
    {
        memset(buffer, 0, buffer_size);
        int bytes_received = recv(server, buffer, buffer_size, 0);
        if(bytes_received <= 0)
        {
            cout << "client disconnected ......... " << endl;
            break;
        }
        cout << "client : " << buffer << endl;
        cout << "server : ";
        memset(buffer, 0, buffer_size);
        cin.getline(buffer, buffer_size);
        send(server, buffer, strlen(buffer), 0);
    }
    close(server);
    close(client);
    return 0;
}