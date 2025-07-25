#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <netdb.h> 

int main(int argc, char const* argv[]) {

    const char* server_hostname = argv[1];
    struct hostent *server = gethostbyname(server_hostname);

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);

    memcpy(&serverAddress.sin_addr.s_addr, server->h_addr, server->h_length);

    connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    const char* message = "Hello, server!";
    send(clientSocket, message, strlen(message), 0);

    close(clientSocket);

    return 0;
}
