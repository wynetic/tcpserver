#include "server/TCPServer.h"

int main() {
    TCPServer server = TCPServer(5400);
    server.startListen();
    return 0;
}
