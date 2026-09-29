# fileTransferTCPServerCpp

A lightweight TCP-based file transfer application written in C++ that allows users to send and receive files between a server and client over a network.

## Table Of Contents
- [Overview](#overview)
- [Prerequisites](#prerequisites)
- [Building the Project](#building-the-project)
- [Running the Application](#running-the-application)
- [How It Works](#how-it-works)
- [Usage Guide](#usage-guide)
- [Technical Details](#technical-details)
- [Limitations](#limitations)
- [Future Enhancements](#future-enhancements)

## Overview

This application operates in two modes:
- **Server Mode**: Listens for incoming connections and can send/receive files
- **Client Mode**: Connects to a server and can send/receive files

Once connected, both server and client can send or receive files to/from each other.

## Prerequisites

Before building this project, ensure having the following installed:

- **C++ Compiler** (g++ with C++11 support or higher)
- **Make**

### Installing Prerequisites

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install build-essential
```

**macOS:**
```bash
xcode-select --install
```

**Windows:**
It can be done in two ways:
- 1. Install [WSL (Windows Subsystem for Linux)](https://docs.microsoft.com/en-us/windows/wsl/install) and follow Ubuntu/Debian instructions
or
- 2. Install [MinGW-w64](https://www.mingw-w64.org/)

## Building the Project

```bash
# Clone the repository 
git clone this repository 
cd fileTransferTCPServerCpp

# Build the project
make

# Run the application
make run
```

### Build Commands

| Command | Description |
|---------|-------------|
| `make` | Build the project (default) |
| `make all` | Same as `make` |
| `make run` | Build and run the application |
| `make clean` | Remove all build artifacts |
| `make rebuild` | Clean and build from scratch |
| `make debug` | Build with debugging symbols |
| `make show` | Display detected source files |

### Build Output

After successful compilation, the executable will be located at:
```
bin/server
```

## Running the Application

### Method 1: Using Make
```bash
make run
```

### Method 2: Direct Execution
```bash
./bin/server
```

### Method 3: Build then Run Separately
```bash
make
./bin/server
```

## How It Works
### File Transfer Protocol

When sending a file, the following data is transmitted in order:

1. **Filename Length** (4 bytes) - uint32_t
2. **Filename** (variable bytes) - actual filename string
3. **File Size** (8 bytes) - size_t
4. **File Data** (chunks of 8KB) - binary file content

The file and saves it as `received_<original_filename>` in the `/bin` folder.

### Project Structure

```
├── 
├── include/
│   ├── fileTransfer.hpp        # Class declaration
│   └── design.hpp              # UI/Menu design utilities
├── src/
│   ├── fileTransfer.cpp        # Class implementation
│   └──main.cpp                 # Entry point, handles user menu
├── bin/                        # Compiled executable output
└── Makefile                    # Build configuration
```

### Key Components

#### `fileTransfer` Class

**Private Members:**
- `server_fd`: Server socket file descriptor
- `client_fd`: Client socket file descriptor  
- `port`: Connection port (default: 8080)
- `ip`: Server IP address (default: 127.0.0.1)

**Public Methods:**
- `createSocket()`: Creates a TCP socket
- `bindSocket()`: Binds server to port 8080
- `listenForConnections()`: Starts listening for clients
- `acceptClient()`: Accepts incoming client connection
- `connectToServer()`: Connects client to server
- `getIpFromUser()`: Prompts user for server IP address
- `sendFile()`: Sends a file to connected peer
- `receiveFile()`: Receives a file from connected peer
- `interactiveMode()`: Menu loop for send/receive operations

## Usage Guide

### Starting a File Transfer Session

**Terminal 1 (Server):**
```bash
./bin/server
# Choose option [0] Connect as server
```

**Terminal 2 (Client):**
```bash
./bin/server
# Choose option [1] Connect as client
# Enter server IP (or press Enter for localhost)
```

### Sending/Receiving Files

Once connected, both server and client will see:
```
[0] Send File
[1] Receive File
[2] Exit
```

- To send: Choose [0], enter full file path
- To receive: Choose [1], wait for incoming file

## Technical Details

- **Protocol**: TCP (SOCK_STREAM)
- **Address Family**: IPv4 (AF_INET)
- **Port**: 8080
- **Buffer Size**: 8192 bytes (8KB chunks)
- **File Mode**: Binary (handles all file types)
- **Progress Tracking**: Real-time percentage display

### Important Notes

1. **Both peers must be ready**: When one side chooses "Send File", the other must choose "Receive File"
2. **Files are saved with prefix**: Received files are saved as `received_<filename>` in the `/bin` folder
3. **Full path required**: When sending, provide the complete file path
4. **Network requirement**: For remote connections, ensure port 8080 is not blocked by firewall

### Dependencies

- Standard C++ libraries
- POSIX socket API (Linux/Unix)
- C++11 or higher

## Limitations

- Single client connection at a time
- No encryption
- No authentication mechanism
- Works on Linux/Unix systems only (uses POSIX sockets)

## Future Enhancements

- Multi-client support
- File encryption
- Resume interrupted transfers
- GUI interface
