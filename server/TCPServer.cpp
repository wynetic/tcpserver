#include "TCPServer.h"
#include "Consts.h" // server settings
#include "../utils/Log.h"
#include "../utils/Settings.h"
#include "ParseRequest.h"
#include "HTTPResponse.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <algorithm>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

TCPServer::TCPServer(int port) : server_port(port), master_socket() {
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(server_port);
    s_addr.sin_addr.s_addr = INADDR_ANY;

    bzero(buffer, sizeof(buffer));
    client_socket = new int[Consts::MAX_CLIENTS];

    if (startServer() != 0) {
        Log::log("Server start failed");
    }
}

TCPServer::~TCPServer() {
    endServer();
}


int TCPServer::startServer() {
    // Clear log file
    fstream file;
    file.open("log.txt", ios::out | ios::trunc);
    file.close();
    /* /////////// */

    Log::log("*** Starting the server... ***");
    master_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (master_socket == -1) {
        Log::log("socket() failed");
        return 1;
    }

    socketSettingUp();

    if (bind(master_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) {
        Log::log("bind() failed");
        close(master_socket);
        return 1;
    }
    return 0;
}


void TCPServer::socketSettingUp() {
    const int enabled = 1;
    if (setsockopt(master_socket, SOL_SOCKET,  SO_REUSEADDR, &enabled, sizeof(int)) == -1)
        Log::log("setsockopt() failed");
}

int TCPServer::startListen() {
    Log::log("Listening to the master socket...");
    if (listen(master_socket, 5) == -1) {
        Log::log("listen() failed");
        return 1;
    }

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        for (int i = 0; i < Consts::MAX_CLIENTS; i++) {
            if (client_socket[i] > 0) {
                FD_SET(client_socket[i], &readfds);
                if (client_socket[i] > max_sd)
                    max_sd = client_socket[i];
            }
        }

        Log::log("Waiting on select...");
        int retval = select(max_sd + 1, &readfds, nullptr, nullptr, nullptr);
        if (retval == -1) {
            Log::log("select() failed");
            return 1;
        } else {
            if (FD_ISSET(master_socket, &readfds)) {
                Log::log("Master socket is readable. Accepting connections...");

                if (acceptConnection() != 0) {
                    endServer();
                }
                
                for (int i = 0; i < Consts::MAX_CLIENTS; i++) {
                    if (FD_ISSET(client_socket[i], &readfds)) {
                        Log::log("Descriptor " + to_string(client_socket[i]) + " is readable");
                        
                        if (getRequest(i) != 0) {
                            closeConnection(i);
                            continue;
                        }
                        
                        if (sendResponse(i) != 0) {
                            closeConnection(i);
                            continue;
                        }
                    }
                } // for ()
            }
        } // else
    }
    return 0;
}

int TCPServer::acceptConnection() {
    int new_socket = accept(master_socket, nullptr, nullptr);
    if (new_socket == -1) {
        Log::log("accept() failed");
        return 1;
    }

    Log::log("New incoming connection: " + to_string(new_socket));
    FD_SET(new_socket, &readfds);

    for (int i = 0; i < Consts::MAX_CLIENTS; i++) {
        if (client_socket[i] == 0) {
            client_socket[i] = new_socket;
            break;
        }
    }
    return 0;
}

void TCPServer::closeConnection(int socket_index) {
    FD_CLR(client_socket[socket_index], &readfds);
    if (client_socket[socket_index] == max_sd) {
        while (FD_ISSET(max_sd, &readfds) == false)
            max_sd -= 1;
    }
    shutdown(client_socket[socket_index], 2);
    close(client_socket[socket_index]);
    client_socket[socket_index] = 0;
}

int TCPServer::getRequest(int socket_index) {    
    bytes_recv = recv(client_socket[socket_index], buffer, sizeof(buffer), 0);
    if (bytes_recv < 0) {
        Log::log("recv() failed");
        closeConnection(socket_index);
        return 1;
    }

    if (bytes_recv == 0) {
        Log::log("Connection closed: " + to_string(client_socket[socket_index]));
        closeConnection(socket_index);
        return 0;
    }

    Log::log(to_string(bytes_recv) + " bytes received");
    Log::log(buffer);
    return 0;
} 

int TCPServer::sendNotFound(int c_socket) {
    HTTPResponse NotFound = HTTPResponse(404, "Closed");
    bytes_sent = send(c_socket, NotFound.getFinalResponse(), strlen(NotFound.getFinalResponse()), 0);
    Log::log(to_string(bytes_sent) + " bytes sent");
    if (bytes_sent != -1) {
        Log::log("Connection closed [404 NOT FOUND]");
        bzero(buffer, sizeof(buffer));
        return 1;
    }
    return 0;
}

int TCPServer::sendNotImplemented(int c_socket) {
    HTTPResponse NotImplemented = HTTPResponse(501, "Closed");
    bytes_sent = send(c_socket, NotImplemented.getFinalResponse(), strlen(NotImplemented.getFinalResponse()), 0);
    Log::log(to_string(bytes_sent) + " bytes sent");
    if (bytes_sent != -1) {
        Log::log("Connection closed [501 NOT IMPLEMENTED]");
        bzero(buffer, sizeof(buffer));
        return 1;
    }
    return 0;
}

int TCPServer::sendTextFile(int c_socket, string URI, string ct) {
    HTTPResponse response = HTTPResponse(200, "Closed", ct);
        
    ifstream in(URI, ifstream::in);

    if (in.is_open()) {
        ostringstream index;
        index << in.rdbuf();
        response.addData(index.str());
    } else {
        if (sendNotFound(c_socket) == 1)
            return 1;
    }
    
    bytes_sent = send(c_socket, response.getFinalResponse(), strlen(response.getFinalResponse()), 0);
    Log::log(to_string(bytes_sent) + " bytes sent");
    if (bytes_sent != -1) {
        Log::log("Connection closed (" + URI + ")");
        bzero(buffer, sizeof(buffer));
        return 1;
    }
    return 0;
}

int TCPServer::sendBinaryFile(int c_socket, string URI, string ct) {
    ifstream in(URI, ios::in | ios::binary | ios::ate);
    
    if (in.is_open()) {
        HTTPResponse response = HTTPResponse(200, "Closed", ct);
        bytes_sent = send(c_socket, response.getFinalResponse(), strlen(response.getFinalResponse()), 0);
        Log::log(to_string(bytes_sent) + " bytes sent");

        streampos size = in.tellg();
        char* file = new char[size];
        in.seekg (0, ios::beg);
        in.read (file, size);
        in.close();

        bytes_sent = send (c_socket, file, size, 0);
        Log::log(to_string(bytes_sent) + " bytes sent");
        if (bytes_sent != -1) {
            Log::log("Connection closed (" + URI + ")");
            bzero(buffer, sizeof(buffer));
            delete file;
            return 1;
        }
    } else {
        if (sendNotFound(c_socket))
            return 1;
    }
    return 0;
}

int TCPServer::sendResponse(int socket_index) {
    string http_method = rp::getHTTPMethod(string(buffer));
    string URI = rp::getURI(string(buffer));
    string con_type = rp::getFileExtension(URI);

    if (http_method.compare("GET") != 0) {
        if (sendNotImplemented(client_socket[socket_index]))
            return 1;
    }

    if (rp::isCGI(URI)) {
        string qstring = rp::getQueryString(URI);
        string path = Consts::CGI_BIN + rp::rmQueryString(URI);

        map<string, string> params = rp::parseQueryString(qstring);

        for (auto iter = params.rbegin(); iter != params.rend(); iter++) {
            string key = "CGI_" + iter->first;
            transform(key.begin(), key.end(), key.begin(), ::toupper);
            const string env = key + "=" + iter->second;
            
            int len = env.length();
            char* param = new char[len];
            for (int i = 0; i < len; i++)
                param[i] = env[i];
            putenv(param);
        }

        pid_t pid = fork();
        if (pid == -1) {
            Log::log("fork() failed");
        }
        if (pid == 0) {
            if (execlp(path.c_str(), path.c_str(), to_string(client_socket[socket_index]).c_str(), nullptr) == -1) {
                Log::log("exec() failed");
            }
        }
        wait(nullptr);
        if (sendTextFile(client_socket[socket_index], "cgi-bin/tmp.txt", "txt")) {
            remove("cgi-bin/tmp.txt");
            return 1;
        }
    }
    
    if (con_type.compare("html") == 0 ||
        con_type.compare("css") == 0 ||
        con_type.compare("js") == 0 ||
        con_type.compare("txt") == 0) {

        if (sendTextFile(client_socket[socket_index], Consts::SERVER_DATA_PATH + URI, con_type))
            return 1;
    } else if (con_type.compare("jpeg") == 0 ||
        con_type.compare("jpg") == 0 ||
        con_type.compare("png") == 0 ||
        con_type.compare("tiff") == 0 ||
        con_type.compare("gif") == 0 || 
        con_type.compare("ico") == 0 ||
        con_type.compare("webp") == 0 ||
        con_type.compare("svg") == 0 ||
        con_type.compare("pdf") == 0 ||
        con_type.compare("djvu") == 0 ||
        con_type.compare("json") == 0 ||
        con_type.compare("js") == 0 ||
        con_type.compare("zip") == 0 ||
        con_type.compare("ogg") == 0 ||
        con_type.compare("wav") == 0 ||
        con_type.compare("webm") == 0 ||
        con_type.compare("mp4") == 0 ||
        con_type.compare("mpeg") == 0) {
        
        if (URI == Consts::FAVICON) {
            if (sendBinaryFile(client_socket[socket_index], Consts::SERVER_DATA_PATH + URI, con_type))
                return 1;
        }

        if (sendBinaryFile(client_socket[socket_index], Consts::SERVER_DATA_PATH + URI, con_type))
            return 1;
    } else {
        if (sendNotFound(client_socket[socket_index]))
            return 1;
    }
    
    bzero(buffer, sizeof(buffer));
    return 0;
}

void TCPServer::endServer() {
    for (int sd = 0; sd <= max_sd; sd++) {
        if (FD_ISSET(sd, &readfds))
            close(sd);
    }
    shutdown(master_socket, 2);
    close(master_socket);
    Log::log("Server shutdown");
    exit(0);
}
