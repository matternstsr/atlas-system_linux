#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define RESPONSE_MSG "HTTP/1.1 200 OK\r\n\r\n"

/**
* main - Entry point of the HTTP server 
* Return: 0 on success 
*/
int main(void)
{
	int socket_fd, client_sock;
	size_t received_bytes = 0;
	char req_buffer[4096], request_method[50], request_path[50], http_version[50];
	char sent[32] = RESPONSE_MSG;
	struct sockaddr_in server_addr;
	socklen_t addr_len = sizeof(server_addr);

	/* Create socket */
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	/* Configure server address */
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	/* Bind the socket */
	if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("Binding failed");
		close(socket_fd);
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port 8080\n");

	/* Start listening for connections */
	if (listen(socket_fd, 5) < 0)
	{
		perror("Listening failed");
		close(socket_fd);
		exit(EXIT_FAILURE);
	}

	/* Main loop to accept and handle clients */
	while (1)
	{
		client_sock = accept(socket_fd, (struct sockaddr *)&server_addr, &addr_len);
		if (client_sock < 0)
		{
			perror("Accepting connection failed");
			continue; /* Continue to the next iteration on error */
		}

		printf("Client connected: %s\n", inet_ntoa(server_addr.sin_addr));

		/* Receive request */
		received_bytes = recv(client_sock, req_buffer, sizeof(req_buffer) - 1, 0);
		if (received_bytes > 0)
		{
			req_buffer[received_bytes] = '\0'; /* Null-terminate the received string */
			printf("Raw request: \"%s\"\n", req_buffer);
			
			/* Parse the request */
			sscanf(req_buffer, "%s %s %s", request_method, request_path, http_version);
			printf("Method: %s\nPath: %s\nVersion: %s\n", request_method, request_path, http_version);
		}

		/* Send response */
		send(client_sock, sent, sizeof(sent) - 1, 0);
		close(client_sock); /* Close the client connection */
	}

	close(socket_fd); /* Close the server socket */
	return (0);
}
