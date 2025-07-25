#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>


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

    // Accept a connection
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == -1) {
        std::cerr << "accept error" << std::endl;
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    //Receive and Send data
    char buffer[1024] = {0};
    ssize_t bytes = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytes == -1) {
        std::cout << "recv failed " << std::endl;
    } else if (bytes == 0) {
        std::cout << "client disconnect" << std::endl;
    } else {
        std::cout << "Message from client: " << buffer << std::endl;
    }
    

    // Close the sockets

    close(serverSocket);
    close(clientSocket);
    return 0;
}
