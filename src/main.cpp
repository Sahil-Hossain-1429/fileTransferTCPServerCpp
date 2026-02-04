#include <iostream>
#include "../include/fileTransfer.hpp"
#include "../include/design.hpp"

using namespace std;
using namespace MenuDesigns;

void displayMenu()
{
    cout << TOPDESIGN << "\n";
    printLineWithDesign("Connect As: ");
    printLineWithDesign("[0] Connect as server");
    printLineWithDesign("[1] Connect as client");
    printLineWithDesign("[2] Exit");
    cout << BOTTOMDESIGN;
    cout << "\n➤ Your choice: ";
}

int main()
{
    int serverOrClientChoice;
    bool running = true;

    fileTransfer server;
    fileTransfer client;

    while (running)
    {
        displayMenu();
        cin >> serverOrClientChoice;
        cin.ignore();

        switch (serverOrClientChoice)
        {
        case 0:
        {
            if (!server.createSocket() || !server.bindSocket() ||
                !server.listenForConnections() || !server.acceptClient())
            {
                printInfoMessage("Server setup failed. Returning to menu...\n");
                break;  // Return to menu
            }

            server.interactiveMode();
            printInfoMessage("Server session ended. Returning to menu...\n");
        }
        break;

        case 1:
        {
            client.getIpFromUser();

            if (!client.connectToServer())
            {
                printInfoMessage("Connection failed. Returning to menu...\n");
                break;  // Return to menu
            }
            
            client.interactiveMode();
            printInfoMessage("Client session ended. Returning to menu...\n");
        }
        break;

        case 2:
        {
            running = false;
            printInfoMessage("Exiting program...\n");
        }
        break;

        default:
            printInfoMessage("Invalid Choice! Please select 0, 1, or 2\n");
            break;
        }
    }
    
    return 0;
}