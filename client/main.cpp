#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <netdb.h> 
#include <vector>
#include <fstream>
#include <cstdint>
#include <filesystem>

#include "key_exchange.h"
#include "../common/shared.h"
#include "../common/file_transfer.h"

int main(int argc, char const* argv[]) {

    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <hostname> <filepath>\n" << std::endl;
        exit(EXIT_FAILURE);
    };

    const char* server_hostname = argv[1];
    struct hostent *server = gethostbyname(server_hostname);

    if (server == NULL) {
        std::cout << "No host: " << server_hostname << std::endl;
        exit(EXIT_FAILURE);
    };

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "socket error" << std::endl;
        exit(EXIT_FAILURE);
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    memcpy(&serverAddress.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "connection error" << std::endl;
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    std::cerr << "Connected to: " << server_hostname << std::endl;

    send_file(clientSocket, argv[2]);


    close(clientSocket);

    return 0;
}
