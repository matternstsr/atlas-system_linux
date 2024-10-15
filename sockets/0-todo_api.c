#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main(void)
{
	int sockfd, newsockfd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_len = sizeof(client_addr);
	char buffer[BUFFER_SIZE];
	ssize_t bytes_received;

	/* Create a socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	/* Prepare the sockaddr_in structure */
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET; /* IPv4 */
	server_addr.sin_addr.s_addr = INADDR_ANY; /* Any address */
	server_addr.sin_port = htons(PORT); /* Port number */

	/* Bind the socket */
	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("Bind failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	/* Listen for incoming connections */
	if (listen(sockfd, 5) < 0)
	{
		perror("Listen failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	printf("Server listening on port %d\n", PORT);

	/* Accept an incoming connection */
	newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);
	if (newsockfd < 0)
	{
		perror("Accept failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	/* Receive a message from the client */
	bytes_received = recv(newsockfd, buffer, sizeof(buffer) - 1, 0);
	if (bytes_received < 0)
		perror("Receive failed");
	else
	{
		buffer[bytes_received] = '\0'; /* Null-terminate the received string */
		printf("Message received: \"%s\"\n", buffer);
	}

	/* Close the connections */
	close(newsockfd);
	close(sockfd);
	return 0;
}
