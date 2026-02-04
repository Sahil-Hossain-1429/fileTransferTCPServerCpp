#ifndef FILETRANSFER_H
#define FILETRANSFER_H

#include <string>       
#include <cstring>      // For memset() to zero out structures and other C string functions
#include <iostream>     
#include <fstream>      // File stream classes for file I/O operations
#include <unistd.h>     // POSIX API: close() function and other system calls
#include <arpa/inet.h>  // Functions for IP address conversion (inet_pton(), inet_ntoa())
#include <sys/socket.h> // Core socket functions (socket(), bind(), listen(), accept(), send(), recv())
#include <netinet/in.h> // Internet address structures (sockaddr_in, INADDR_ANY, htons())

class fileTransfer
{
private:
    int maxClients;
    int server_fd;
    int client_fd;
    int port = 8080;              // PORT to connect to server
    std::string ip = "127.0.0.1"; // Default local address IP to connect to server

    struct sockaddr_in serverListenerAddress, // used for server operations
        serverConnectAddr;                    // used for client operations

    std::string infoMsg = "";

public:
    fileTransfer();
    ~fileTransfer();

    bool bindSocket();
    bool acceptClient();
    bool createSocket();
    bool listenForConnections();

    void run();
    bool sendFile();
    bool receiveFile();
    void interactiveMode();

    void getIpFromUser();
    bool connectToServer();

    void closeClient();
    void closeServerConnections();
};

#endif