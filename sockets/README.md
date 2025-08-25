# Socket Chat Application

A simple client-server chat application implemented in C using socket
programming. This project demonstrates basic network communication
concepts including TCP sockets, connection handling, and bidirectional
messaging.

## Overview

This project consists of two main components:
- **Server**: Listens for incoming connections and handles client
  communication
- **Client**: Connects to the server and enables interactive messaging

The application provides a basic chat functionality where clients can
send messages to the server and receive responses. The conversation
continues until either party sends "Bye" to terminate the connection.

## Building the Project

To build both the server and client executables:

```bash
make
```

To clean build artifacts:

```bash
make clean
```

## Usage

### Starting the Server

1. Start the server by providing a port number:
   ```bash
   ./server <port_number>
   ```
   
   Example:
   ```bash
   ./server 8080
   ```

2. The server will start listening for client connections on the specified port.
3. Send "Bye" to a terminate the connection with the client at any time.

### Connecting with the Client

1. In a separate terminal, connect a client to the server:
   ```bash
   ./client <hostname> <port>
   ```
   
   Example:
   ```bash
   ./client localhost 8080
   ```

2. Once connected, you can start exchanging messages:
   - Type your message and press Enter to send
   - The server will respond with its message   

## Project Structure

```
sockets/
├── server.c      # Server implementation
├── client.c      # Client implementation
├── Makefile      # Build configuration
├── server        # Compiled server executable
├── client        # Compiled client executable
└── README.md     # This file
```

## TODO / Future Improvements

1. **Calculator Program**: Implement a calculator service that can perform mathematical operations over the network
2. **Modern API Migration**: Replace deprecated `gethostbyname()` with `getaddrinfo()` for better IPv6 support
7. **Multiple Client Support**: Allow server to handle multiple concurrent clients
4. **C++ Implementation**: Create a C++ version of the network code for comparison

## References

- [Socket Programming in C Tutorial](https://youtube.com/playlist?list=PLPyaR5G9aNDvs6TtdpLcVO43_jvxp4emI&si=InjyoX3xL126qPuM)
- [Linux Socket Programming Documentation](https://man7.org/linux/man-pages/man2/socket.2.html)

