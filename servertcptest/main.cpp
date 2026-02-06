#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <cerrno>

using namespace std;

int main() 
{
    // how to create a socket ?
    // what means listening socket ?
    //=>>>>>>> A listening socket is a socket that waits for incoming connection requests from clients. It's used on the server side to accept new connections.
    int listening = socket(AF_INET, SOCK_STREAM, 0);    // ==> AF_INET - Address family
                                                        // AF_INET = IPv4
                                                        // AF_INET6 = IPv6
                                                        // AF_UNIX = Local communication
                                                        // SOCK_STREAM - Socket type

                                                        // SOCK_STREAM = TCP (reliable, ordered, connection-based)
                                                        // SOCK_DGRAM = UDP (unreliable, connectionless)
                                                        // 0 - Protocol

                                                        // 0 = Let the system choose the appropriate protocol
                                                        // For SOCK_STREAM, it defaults to TCP (IPPROTO_TCP)
                                                        // For SOCK_DGRAM, it defaults to UDP (IPPROTO_UDP)
    // can you check the man socket to more information ....?
    // we should check thr return value to make sure that was created !
    if(listening == -1){
        cerr << "Can't create a socket ! " << endl;
        return -1;}
    
    // Allow reuse of address/port (prevents "Address already in use" error)
    int opt = 1;
    setsockopt(listening, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // after this we should bind the socket to an IP add and port then we can listen for incoming connections!
    // What is Binding a Socket?
    // ====>>> Binding assigns a specific IP address and port number to your socket. It tells the operating system: "When data arrives on this IP:port, send it to this socket."
    sockaddr_in hint;
    hint.sin_family = AF_INET; // Address family (IPv4)
    hint.sin_port = htons(55000); // Port number (convert to network byte order) // htons - Host to Network Short (converts a 16-bit number to network byte order)
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr); // IP address (convert from text to binary form) // inet_pton - Internet address presentation to network address (converts an IP address from text to binary form)
    // what bind does ?
    // ===>>> bind() associates the socket with a specific IP address and port number. It tells the operating system that this socket will be used to receive data sent to that IP:port combination
    if(bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1)
    {
        cerr << "Can't bind to IP/port ! Error: " << strerror(errno) << endl;
        return -2;
    }
    // now we will mark the socket for listening incomming connections
    if(listen(listening, SOMAXCONN) == -1) // somaxconn - Maximum number of pending connections in the queue (defined in <sys/socket.h>)
    {
        cerr << "Can't listen ! " << endl;
        return -3;
    }
    // now we will accept incoming connections
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST]; // Client's remote name
    char service[NI_MAXSERV]; // Service (port) the client is connect on
    
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
    if(clientSocket == -1)
    {
        cerr << "Problem with client connecting ! " << endl;
        return -4;
    }
    // close the listening socket
    close(listening);
    memset(host, 0, NI_MAXHOST); // Clear the host buffer
    memset(service, 0, NI_MAXSERV); // Clear the service buffer
    int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0);
    if(result == 0)
    {
        cout << host << " connected on port " << service << endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST); // Convert the client's IP address from binary to text form // inet_ntop - Internet address network to presentation (converts an IP address from binary to text form)
        cout << host << " connected on port " << ntohs(client.sin_port) << endl; // ntohs - Network to Host Short (converts a 16-bit number from network byte order to host byte order)
    }
    // now while recuving data from the client we will echo it back to the client
    char buf[4096]; 
    while(true)
    {
        // we should clear the buffer before receiving data
        memset(buf, 0, 4096);
        // wait for a message
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if(bytesReceived == -1)
        {
            cerr << "Error in recv() ! " << endl;
            break;
        }
        if(bytesReceived == 0)
        {            cout << "Client disconnected ! " << endl;
            break;
        }
        // display message
        cout << "Received: " << string(buf, 0, bytesReceived) << endl;
        // echo message back to client
        int sendResult = send(clientSocket, buf, bytesReceived + 1, 0);
        if(sendResult == -1)
        {            cerr << "Error in send() ! " << endl;
            break;
        }
        // close the socket
    }
    close(clientSocket);
    return 0;
}