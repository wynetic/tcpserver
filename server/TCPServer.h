#ifndef TCPSERVER_H
#define TCPSERVER_H

#pragma once

#include <netinet/in.h>
#include <string>
#include "Consts.h"


class TCPServer {
    public:
        TCPServer(int port); // constructor
        ~TCPServer(); // destructor
        int startListen();

    private:
        char buffer[1024]; // request buffer
        fd_set readfds; // master set for select
        int* client_socket; // array of client sockets
        int max_sd; // max socket descriptor value
        int server_port;
        int master_socket; // server socket
        int bytes_recv; // bytes recieved from client
        int bytes_sent; // bytes sent to client
        struct sockaddr_in s_addr; // server address
        
        int startServer();
        void socketSettingUp();

        int acceptConnection();
        int getRequest(int socket_index);
        int sendResponse(int socket_index);

        int sendNotFound(int c_socket);
        int sendNotImplemented(int c_socket);
        int httpInternalServerError(int c_socket);

        int sendTextFile(int c_socket, std::string URI, std::string ct);
        int sendBinaryFile(int c_socket, std::string URI, std::string ct);

        void closeConnection(int socket_index);
        void endServer();
};

#endif
