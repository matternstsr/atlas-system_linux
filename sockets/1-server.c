#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 12345
#define BUFFER_SIZE 1024

/**
* main - Initializes and runs a simple TCP server.
* This function creates a TCP socket, binds it to a specified port (12345),
* and listens for incoming connections. Upon accepting a connection, it
* receives messages from clients and sends responses back.
* Return: 0 upon successful execution (though unreachable in this case).
*/
int main(void)
{
	int sockfd, newsockfd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_len = sizeof(client_addr);
	char buffer[BUFFER_SIZE];
	ssize_t bytes_received;

	sockfd = socket(AF_INET, SOCK_STREAM, 0); /* Create a socket */
	if (sockfd < 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&server_addr, 0, sizeof(server_addr)); /* prep the sockaddr_in */
	server_addr.sin_family = AF_INET; /* IPv4 */
	server_addr.sin_addr.s_addr = INADDR_ANY; /* Any address */
	server_addr.sin_port = htons(PORT); /* Port number */

	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{ /* Bind the socket */
		perror("Bind failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	if (listen(sockfd, 5) < 0) /* Listen for incoming connections */
	{
		perror("Listen failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	printf("Server listening on port %d\n", PORT);
	while (1) /* Accept incoming connections in a loop */
	{
		newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);
		if (newsockfd < 0) /* Accept a connection */
		{
			perror("Accept failed");
			continue;
		}
		printf("Client connected\n");
		while (1)
		{ /* Receive messages from the client */
			bytes_received = recv(newsockfd, buffer, sizeof(buffer) - 1, 0);
			if (bytes_received < 0)
			{
				perror("Receive failed");
				break;
			}
			else if (bytes_received == 0)
			{
				printf("Client disconnected\n"); /* Client disconnected */
				break;
			}
			buffer[bytes_received] = '\0'; /* received string gets null */
			printf("Received message: %s\n", buffer);
			send(newsockfd, buffer, bytes_received, 0); /* Echo to the client */
		}
		close(newsockfd); /* Close the connection to the client */
	}
	close(sockfd); /* Close the main socket (not reachable) */
	return (0);
}
