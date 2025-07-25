#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>


int main() {

    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Define the server address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    
    // Bind the socket to IP and port
    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    // Listen for incoming connections
    listen(serverSocket, 5);

    // Accept a connection
    int clientSocket = accept(serverSocket, nullptr, nullptr);

    //Receive and Send data
    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Message from client: " << buffer << std::endl;

    // Close the sockets

    close(serverSocket);
    close(clientSocket);
    return 0;
}
