#include "../include/server.hpp"

using namespace std;
TcpServer::TcpServer()
{
    maxClients = 1;
    server_fd = -1;
    client_fd = -1;
    memset(&address, 0, sizeof(address));
}

TcpServer::~TcpServer()
{
    closeServerConnections();
}

bool TcpServer::createSocket()
{
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("Socket failed");
        return false;
    }
    /*
        * int sockfd declares an integer variable that will store the socket file descriptor.
        * AF_INET indicates the socket will use the IPv4 address family.
        * SOCK_STREAM specifies that the socket will use TCP (a stream-oriented protocol) and,
        * 0 lets the system choose the default protocol for the specified address family and socket type (which is TCP in this case).
    */

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    return true;
}

bool TcpServer::bindSocket()
{
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind Failed");
        close(server_fd);
        return false;
    }
    return true;
}

bool TcpServer::listenForConnections()
{
    if (listen(server_fd, maxClients) < 0)
    {
        perror("Listen failed");
        close(server_fd);
        return false;
    }

    cout << "Server listening on port 8080..." << "\n";
    return true;
}

bool TcpServer::acceptClient()
{
    struct sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);

    client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_len);
    if (client_fd < 0)
    {
        perror("Accept failed");
        close(server_fd);
        return false;
    }

    cout << "Client connected!" << "\n";
    return true;
}

void TcpServer::sendMessage()
{
    const char *message = "+--------------------------+\n"
                          "|    Hello from server!    |\n"
                          "|  Now Enter Your Message: |\n"
                          "+--------------------------+\n";

    send(client_fd, message, strlen(message), 0);
    cout << "Message sent to client" << "\n";
}

void TcpServer::closeClient()
{
    if (client_fd >= 0)
    {
        close(client_fd);
        client_fd = -1;
        cout << "Client Disconnected" << "\n";
    }
}

void TcpServer::closeServerConnections()
{
    closeClient();

    if (server_fd >= 0)
    {
        close(server_fd);
        server_fd = -1;
    }

    cout << "Server shutdown complete" << "\n";
}

string TcpServer::receiveRequest()
{
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));

    int bytesReceived = recv(client_fd, buffer, sizeof(buffer), 0);

    if (bytesReceived < 0)
    {
        perror("Receive Failed");
        return "";
    }

    if (bytesReceived == 0)
    {
        cout << "Client disconnected during receive" << "\n";
        return "";
    }

    cout << "--- Received " << bytesReceived << " bytes ---\n";
    cout << buffer << "\n";
    cout << "--- End of request ---\n\n";

    return string(buffer);
}