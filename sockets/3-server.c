#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 12345
#define BUFFER_SIZE 1024

/*
* main - Initializes and runs a simple TCP server.
* This function creates a TCP socket, binds it to a specified port (12345),
* listens for incoming connections, and handles client requests in a loop.
* Return: 0 upon successful execution.
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
	{
		perror("Bind failed"); /* Bind the socket */
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	if (listen(sockfd, 5) < 0)
	{ /* Listen on the socket */
		perror("Listen failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port %d\n", PORT);

	while (1)
	{ /* Accept client connections indefinitely */
		newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);
		if (newsockfd < 0)
		{
			perror("Accept failed");
			continue; /* Handle error but keep the server running */
		}

		printf("Client connected\n");

		/* Receive messages from the client */
		while ((bytes_received = recv(newsockfd, buffer, sizeof(buffer) - 1, 0)) > 0)
		{
			buffer[bytes_received] = '\0'; /* Null-terminate received string */
			printf("Received: %s\n", buffer);
			
			/* Echo the message back to the client */
			send(newsockfd, buffer, bytes_received, 0);
		}

		if (bytes_received < 0)
			perror("Receive failed");

		printf("Client disconnected\n");
		close(newsockfd); /* Close the connection with the client */
	}

	close(sockfd); /* Close the server socket (unreachable) */
	return 0;
}
