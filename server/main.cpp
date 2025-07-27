#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <fstream>

int main() {

    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "socket error" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Define the server address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    
    // Bind the socket to IP and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "bind error" << std::endl;
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "listen failed" << std::endl;
        close(serverSocket);
        exit(EXIT_FAILURE);
    };

    while (true) {

        // Accept a connection
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == -1) {
            std::cerr << "accept error" << std::endl;
            close(serverSocket);
            exit(EXIT_FAILURE);
        }

        // Recieve filename length
        uint16_t name_length;
        recv(clientSocket, &name_length, sizeof(name_length), 0);


        std::cout << "name length:  " << name_length << std::endl;
        // Recieve filename

        // char filename_buffer[name_length];
        std::vector<char> filename_buffer(name_length);
        recv(clientSocket, filename_buffer.data(), name_length, 0);

        std::string filename(filename_buffer.begin(), filename_buffer.end());
        std::cout << "filename:  " << filename << std::endl;

        // Recieve filesize
        uint64_t filesize;
        recv(clientSocket, &filesize, sizeof(uint64_t), 0);
        std::cout << "filesize:  " << filesize << std::endl;

        // File to save the data

        std::ofstream file(filename, std::ios::binary);
        



        //Receive and Send data
        
        const size_t BUFFER_SIZE = 4096;
        std::vector<char> buffer(BUFFER_SIZE);

        ssize_t bytes;

        while ((bytes = recv(clientSocket, buffer.data(), buffer.size(), 0)) > 0) {
            std::fill(buffer.begin() + bytes, buffer.end(), 0);
            file.write(buffer.data(), bytes);
        }

        if (bytes == -1) {
            std::cout << "recv failed " << std::endl;
        } else if (bytes == 0) {
            std::cout << "client disconnect" << std::endl;
        } else {
            std::cout << "Message from client: " << buffer.data() << std::endl;
        }

        close(clientSocket);
        std::cout << "client disconnect" << std::endl;
    }



    

    // Close the sockets

    close(serverSocket);
    return 0;
}
