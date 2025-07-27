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


struct file_header {
    std::string filename;
    uint64_t file_size;
};


int send_file(int socket, const std::string& filepath) {
    
    std::ifstream file(filepath, std::ios::binary);

    if (!file.is_open()) {
        std::cout << "Error opening file" << std::endl;
        return -1;
    }

    file_header header;
    header.file_size = std::filesystem::file_size(filepath);
    header.filename = std::filesystem::path(filepath).filename().string();

    // std::cout << "name: " << header.filename << " size: " << header.file_size << std::endl;

    // Send filename length
    uint16_t name_length = header.filename.size();
    std::cout << "name length:  " << name_length << std::endl;
    send(socket, &name_length, sizeof(name_length), 0);

    // Send filename
    std::cout << "filename:  " << header.filename.c_str() << std::endl;
    send(socket, header.filename.c_str(), name_length, 0);

    // Send filesize
    std::cout << "filesize:  " << header.file_size << std::endl;
    send(socket, &header.file_size, sizeof(header.file_size), 0);

    const size_t BUFFER_SIZE = 4096;
    std::vector<char> buffer(BUFFER_SIZE);

    while (file) {

        file.read(buffer.data(), buffer.size());
        std::streamsize bytes_read = file.gcount();

        if (bytes_read > 0) {

            if (bytes_read < buffer.size()) {
                std::fill(buffer.begin() + bytes_read, buffer.end(), 0);
            }

            send(socket, buffer.data(), bytes_read, 0);
        }

    }
}

int main(int argc, char const* argv[]) {

    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <hostname>\n" << std::endl;
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
    serverAddress.sin_port = htons(8080);

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
