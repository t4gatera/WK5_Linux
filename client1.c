#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define HOST "127.0.0.1"

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE];

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, HOST, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Connected to the server.\n");

    // Chat loop
    while (1) {
        // Send message to server
        printf("Client: ");
        fgets(message, BUFFER_SIZE, stdin);
        send(sockfd, message, strlen(message), 0);

        // Check for "bye" to end the chat
        if (strncmp(message, "bye", 3) == 0) {
            printf("Client ended the chat.\n");
            break;
        }

        // Receive message from server
        memset(buffer, 0, BUFFER_SIZE);
        int valread = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (valread <= 0) {
            printf("Server disconnected.\n");
            break;
        }
        printf("Server: %s\n", buffer);

        // Check for "bye" to end the chat
        if (strncmp(buffer, "bye", 3) == 0) {
            printf("Server ended the chat.\n");
            break;
        }
    }

    // Close the socket
    close(sockfd);
    return 0;
}

