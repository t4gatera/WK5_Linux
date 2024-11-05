#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"  // Replace with server's IP
#define SERVER_PORT 8080       // Replace with server's port

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    const char *message = "Hello, Server!";
    char buffer[1024];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
}
