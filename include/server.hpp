#ifndef SERVER_H
#define SERVER_H

#include <iostream>     // For print to console
#include <sys/socket.h> // Core socket functions
#include <netinet/in.h> // internet address structures
#include <unistd.h>     //For close() function
#include <cstring>      //For memset() to zero out structures
#include <cerrno>       //For error handling

class TcpServer
{
private:
    int maxClients;
    int server_fd;
    int client_fd;
    struct sockaddr_in address;

public:
    TcpServer();
    ~TcpServer();  // Add destructor for cleanup
    
    bool createSocket();
    bool bindSocket();
    bool listenForConnections();
    bool acceptClient();
    void sendMessage();
    void closeClient();
    void closeServerConnections();
    std::string receiveRequest();
};


#endif


    
        
        
