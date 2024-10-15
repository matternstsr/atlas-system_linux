#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

/**
* main - Connects to a specified server.
* This function creates a TCP socket, connects to the server at the
* provided host and port, and allows the user to send messages to the server.
* The program expects two command-line arguments: the host and port number.
* If the arguments are insufficient, it prints usage information and exits.
* 
* Return: 0 upon successful execution.
*/
int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in server_addr;
	char buffer[BUFFER_SIZE];

	/* Check if the correct number of arguments is provided */
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	const char *host = argv[1]; /* Convert port to unsigned short */
	unsigned short port = (unsigned short)atoi(argv[2]);

	sockfd = socket(AF_INET, SOCK_STREAM, 0); /* Create a socket */
	if (sockfd < 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&server_addr, 0, sizeof(server_addr)); /* Prepare sockaddr_in */
	server_addr.sin_family = AF_INET; /* IPv4 */
	server_addr.sin_port = htons(port); /* Port number */

	/* Convert IP address from text to binary form */
	if (inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0)
	{
		perror("Invalid address/ Address not supported");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	/* Connect to the server */
	if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("Connection failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	printf("Connected to server at %s:%d\n", host, port);

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
		if (bytes_received < 0)  /* Receive the server's response */
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
