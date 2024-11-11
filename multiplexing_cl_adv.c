#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid address or Address not supported\n");
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection failed\n");
        return -1;
    }

    printf("Connected to the server. Type 'end' to close the connection.\n");

    while (1) {
        // Get two numbers from user input
        printf("Enter two integers (or type 'end' to exit): ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Send message to server
        write(sock, buffer, strlen(buffer));

        // Check if the user wants to end the connection
        if (strncmp(buffer, "end", 3) == 0) {
            printf("Ending connection to the server.\n");
            break;
        }

        // Receive the result from the server
        memset(buffer, 0, BUFFER_SIZE);
        read(sock, buffer, BUFFER_SIZE);
        printf("The sum received from server is: %s\n", buffer);
    }

    close(sock);
    return 0;
}

