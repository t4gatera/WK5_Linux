#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to the port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, addrlen) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Server is listening on port %d...\n", PORT);

    // Accept an incoming connection
    new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Connection established with client.\n");

    // Chat loop
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        
        // Receive message from client
        int valread = recv(new_socket, buffer, BUFFER_SIZE, 0);
        if (valread <= 0) {
            printf("Client disconnected.\n");
            break;
        }
        printf("Client: %s\n", buffer);

        // Check for "bye" to end the chat
        if (strncmp(buffer, "bye", 3) == 0) {
            printf("Client ended the chat.\n");
            break;
        }

        // Send message to client
        printf("Server: ");
        fgets(message, BUFFER_SIZE, stdin);
        send(new_socket, message, strlen(message), 0);

        // Check for "bye" to end the chat
        if (strncmp(message, "bye", 3) == 0) {
            printf("Server ended the chat.\n");
            break;
        }
    }

    // Close the socket
    close(new_socket);
    close(server_fd);
    return 0;
}

