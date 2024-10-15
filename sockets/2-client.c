#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "localhost" /* "????????????" */
#define PORT 12345
#define BUFFER_SIZE 1024

/**
* main - Initializes and runs a simple TCP client.
* This function creates a TCP socket, connects to the server at a specified
* IP address and port, and allows the user to send messages to the server.
* The client receives responses from the server and prints them to the console.
* Return: 0 upon successful execution.
*/
int main(void)
{
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0); /* Create a socket */
    if (sockfd < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&server_addr, 0, sizeof(server_addr)); /* prep the sockaddr_in */
    server_addr.sin_family = AF_INET; /* IPv4 */
    server_addr.sin_port = htons(PORT); /* Port number */

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
    { /* Convert IP address from text to binary form */
        perror("Invalid address/ Address not supported");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    { /* Connect to the server */
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server at %s:%d\n", SERVER_IP, PORT);

    while (1) /* Communicate with the server */
    {
        printf("Enter message (or 'exit' to quit): ");
        fgets(buffer, sizeof(buffer), stdin);

        buffer[strcspn(buffer, "\n")] = 0; /* Remove newline char from input */

        if (strcmp(buffer, "exit") == 0) /* Exit loop if user types "exit" */
        {
            break;
        }

        send(sockfd, buffer, strlen(buffer), 0);/* Send message to the server */

        ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received < 0) /* Receive the server's response */
        {
            perror("Receive failed");
            break;
        }
        buffer[bytes_received] = '\0'; /* Null-terminate the received string */
        printf("Server response: %s\n", buffer);
    }

    close(sockfd); /* Close the socket */
    return 0;
}
