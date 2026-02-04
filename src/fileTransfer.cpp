#include "../include/fileTransfer.hpp"
#include "../include/design.hpp"

using namespace std;
using namespace MenuDesigns;

fileTransfer::fileTransfer()
{
    maxClients = 1;
    server_fd = -1;
    client_fd = -1;

    memset(&serverListenerAddress, 0, sizeof(serverListenerAddress)); // used for server operations
    memset(&serverConnectAddr, 0, sizeof(serverConnectAddr));         // used for client operations

    
}

fileTransfer::~fileTransfer()
{
    closeServerConnections();
}

bool fileTransfer::createSocket()
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

bool fileTransfer::bindSocket()
{
    serverListenerAddress.sin_family = AF_INET;
    serverListenerAddress.sin_addr.s_addr = INADDR_ANY;
    serverListenerAddress.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&serverListenerAddress, sizeof(serverListenerAddress)) < 0)
    {
        perror("Bind Failed");
        close(server_fd);
        return false;
    }
    return true;
}

bool fileTransfer::listenForConnections()
{
    if (listen(server_fd, maxClients) < 0)
    {
        perror("Listen failed");
        close(server_fd);
        return false;
    }

    printInfoMessage("Server listening on port...");
    return true;
}

bool fileTransfer::acceptClient()
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
    printInfoMessage("Client Connected...");
    return true;
}

void fileTransfer::closeClient()
{
    if (client_fd >= 0)
    {
        close(client_fd);
        client_fd = -1;
        printInfoMessage("Client Disconnected...");
    }
}

void fileTransfer::closeServerConnections()
{
    closeClient();

    if (server_fd >= 0)
    {
        close(server_fd);
        server_fd = -1;
    }
    printInfoMessage("Server shutdown complete...");

}

bool fileTransfer::sendFile()
{
    string fileFullPath;
    printInfoMessage("Enter file path with file extension: ");
    getline(cin, fileFullPath);

    ifstream file(fileFullPath, ios::binary); // opening file in binary mode
    if (!file.is_open())
    {
        cerr << "Error: Could not open file: " << fileFullPath << "\n";
        return false;
    }

    file.seekg(0, ios::end); // Get file size
    size_t fileSize = file.tellg();
    file.seekg(0, ios::beg);

    size_t lastSlash = fileFullPath.find_last_of("/\\"); // Extract jsut the fileName (not full path)
    string fileNameOnly = (lastSlash != string::npos) ? fileFullPath.substr(lastSlash + 1) : fileFullPath;

    uint32_t nameLength = fileNameOnly.length(); // Send filename length
    send(client_fd, &nameLength, sizeof(nameLength), 0);

    send(client_fd, fileNameOnly.c_str(), nameLength, 0); // Send filename

    send(client_fd, &fileSize, sizeof(fileSize), 0); // send file size

    infoMsg = "Sending File: " + fileNameOnly + "(" + to_string(fileSize) + " bytes)";
    printInfoMessage(infoMsg);

    const size_t BUFFER_SIZE = 8192; // Send file data in chunks
    char buffer[BUFFER_SIZE];
    size_t totalSent = 0;

    while (file.read(buffer, BUFFER_SIZE) || file.gcount() > 0)
    {
        size_t bytesToSend = file.gcount();
        ssize_t bytesSent = send(client_fd, buffer, bytesToSend, 0);

        if (bytesSent < 0)
        {
            perror("Send Failed");
            file.close();
            return false;
        }

        totalSent += bytesSent;

        float progress = (totalSent * 100.0) / fileSize; // Progress indicator
        infoMsg = "Progress: " + to_string(progress) + "%";
        printInfoMessage(infoMsg);
    }

    file.close();
    printInfoMessage("File sent successfully!");
    return true;
}

bool fileTransfer::receiveFile()
{

    uint32_t nameLength; // receive filename length
    ssize_t bytes = recv(client_fd, &nameLength, sizeof(nameLength), 0);
    if (bytes <= 0)
    {
        cerr << "Error receiving filename length\n";
        return false;
    }

    char *fileNameBuf = new char[nameLength + 1]; // receive filename
    bytes = recv(client_fd, fileNameBuf, nameLength, 0);
    if (bytes <= 0)
    {
        delete[] fileNameBuf;
        cerr << "Error receiving filename\n";
        return false;
    }

    fileNameBuf[nameLength] = '\0';
    string fileName(fileNameBuf);
    delete[] fileNameBuf;

    size_t fileSize; // Receive file size
    bytes = recv(client_fd, &fileSize, sizeof(fileSize), 0);
    if (bytes <= 0)
    {
        cerr << "Error receiving file size\n";
        return false;
    }

    infoMsg = "Receiving File: " + fileName + "(" + to_string(fileSize) + " bytes)";
    printInfoMessage(infoMsg);

    string outputPath = "received_" + fileName; // Creating output file
    ofstream outFile(outputPath, ios::binary);

    if (!outFile.is_open())
    {
        cerr << "Error : could not create output file\n";
        return false;
    }

    const size_t BUFFER_SIZE = 8192; // Receive file data in chunks
    char buffer[BUFFER_SIZE];
    size_t totalReceived = 0;

    while (totalReceived < fileSize)
    {
        size_t bytesToReceive = min(BUFFER_SIZE, fileSize - totalReceived);
        ssize_t bytesReceived = recv(client_fd, buffer, bytesToReceive, 0);

        if (bytesReceived <= 0)
        {
            cerr << "\nError receiving file data\n";
            outFile.close();
            return false;
        }

        outFile.write(buffer, bytesReceived);
        totalReceived += bytesReceived;

        float progress = (totalReceived * 100.0) / fileSize; // Progress indicator
        infoMsg = "Progress: " + to_string(progress) + "%";
        printInfoMessage(infoMsg);
    }

    outFile.close();
    infoMsg = "File received successfully! Saved as: " + outputPath;
    printInfoMessage(infoMsg);
    return true;
}

void fileTransfer::interactiveMode()
{
    int choice;
    bool running = true;

    while (running)
    {

        cout << TOPDESIGN << "\n";

        printLineWithDesign("Select Options Below:");
        printLineWithDesign("[0] Send File");
        printLineWithDesign("[1] Recieve File");
        printLineWithDesign("[2] Exit");

        cout << BOTTOMDESIGN;
        cout << "\n➤ Your choice: ";

        cin >> choice;
        cin.ignore(); // cleared the newline from input buffer

        switch (choice)
        {
        case 0:
            if (!sendFile())
            {
                printInfoMessage("File send failed. Continuing...");
            }
            break;

        case 1:
            if (!receiveFile())
            {
                printInfoMessage("File receive failed. Continuing...");
            }
            break;

        case 2:
            printInfoMessage("Exiting");
            running = false;
            break;

        default:
            cout << "Invalid choice. Please select 0, 1 or 2\n";
            printInfoMessage("Invalid choice.\nPlease select 0, 1, or 2:");
            break;
        }
    }
}

void fileTransfer::getIpFromUser()
{
    string userIp;
    cout << "\n➤ Enter server IP address \n(press Enter for default 127.0.0.1): ";
    getline(cin, userIp);
    
    if (!userIp.empty())
    {
        ip = userIp;
    }
}

bool fileTransfer::connectToServer() // For client to connect
{

    client_fd = socket(AF_INET, SOCK_STREAM, 0); // Create socket
    if (client_fd < 0)
    {
        perror("Socket creation failed");
        return false;
    }

    serverConnectAddr.sin_family = AF_INET; // Setup server address
    serverConnectAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &serverConnectAddr.sin_addr) <= 0) // Convert IP address from text to binary
    {
        perror("Invalid address");
        close(client_fd);
        return false;
    }

    if (connect(client_fd, (struct sockaddr *)&serverConnectAddr, sizeof(serverConnectAddr)) < 0) // Connect to server
    {
        perror("Connection failed");
        close(client_fd);
        return false;
    }

    infoMsg = "Connected to server at ";
    printInfoMessage(infoMsg);

    return true;
}