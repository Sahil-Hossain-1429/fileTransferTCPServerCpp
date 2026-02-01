#include "../include/server.hpp"
#include <iostream>

int main()
{
    TcpServer server;
    if (!server.createSocket() || !server.bindSocket() ||
        !server.listenForConnections() || !server.acceptClient())
    {
        return 1;
    }
    
    // server.sendMessage();

    std::string request = server.receiveRequest();

    server.closeServerConnections();

    return 0;
}