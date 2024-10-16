#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 12345
#define BUFFER_SIZE 1024

/* Function prototypes */
int create_socket(void);
void bind_socket(int sockfd);
void listen_for_connections(int sockfd);
void handle_client(int newsockfd);
void cleanup(int sockfd, int newsockfd);

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
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(client_addr);

	sockfd = create_socket();
	bind_socket(sockfd);
	listen_for_connections(sockfd);
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
		handle_client(newsockfd);
	}

	cleanup(sockfd, -1); /* Close the main socket (not reachable) */
	return (0);
}

/**
* create_socket - Creates a TCP socket.
* This function initializes a socket for IPv4 communication using
* the TCP protocol. It returns the socket file descriptor.
* Return: socket file descriptor on success, or exits on failure.
*/
int create_socket(void)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0); /* Create a socket */

	if (sockfd < 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}
	return (sockfd);
}

/**
* bind_socket - Binds the socket to a specified port.
* This function binds the socket to the specified port (12345)
* and sets it to listen for incoming connections.
* Parameter: sockfd - the socket file descriptor to bind.
* Return: void.
*/
void bind_socket(int sockfd)
{
	struct sockaddr_in server_addr;

	memset(&server_addr, 0, sizeof(server_addr)); /* Prepare the sockaddr_in */
	server_addr.sin_family = AF_INET; /* IPv4 */
	server_addr.sin_addr.s_addr = INADDR_ANY; /* Any address */
	server_addr.sin_port = htons(PORT); /* Port number */

	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{ /* Bind the socket */
		perror("Bind failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
}

/**
* listen_for_connections - Prepares the socket to listen for connections.
* This function puts the socket in a listening state with a backlog of
* up to 5 pending connections.
* Parameter: sockfd - the socket file descriptor to listen on.
* Return: void.
*/
void listen_for_connections(int sockfd)
{
	if (listen(sockfd, 5) < 0) /* Listen for incoming connections */
	{
		perror("Listen failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
}

/**
* handle_client - Manages communication with a connected client.
* This function receives messages from the client, echoes them back,
* and handles client disconnection.
* Parameter: newsockfd - the socket file descriptor for the connected client.
* Return: void.
*/
void handle_client(int newsockfd)
{
	char buffer[BUFFER_SIZE];
	ssize_t bytes_received;

	while (1) /* Receive messages from the client */
	{
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
		buffer[bytes_received] = '\0'; /* Null-terminate the received string */
		printf("Received message: %s\n", buffer);
		send(newsockfd, buffer, bytes_received, 0); /* Echo to the client */
	}
	close(newsockfd); /* Close the connection to the client */
}

/**
* cleanup - Closes the server and client sockets.
* This func ensures that both the server and client sockets closed properly.
* Parameters: sockfd - the server socket file descriptor,
*             newsockfd - the client socket file desc (can be -1 if no client).
* Return: void.
*/
void cleanup(int sockfd, int newsockfd)
{
	if (newsockfd != -1)
		close(newsockfd); /* Close the client socket */
	close(sockfd); /* Close the main socket */
}
