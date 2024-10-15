#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define RESPONSE "HTTP/1.1 200 OK\r\n\r\n"

/* 
* main - Entry point of the HTTP server
* Return: 0 on success 
*/
int main(void)
{
	int server_socket, client_socket;
	size_t received_bytes = 0;
	char buffer[4096];
	char method[50], path[50], version[50], sent[32] = RESPONSE;
	struct sockaddr_in server_addr;
	socklen_t addr_len = sizeof(server_addr);
	
	/* Create socket */
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
	{
		perror("socket failed"),
		exit(EXIT_FAILURE);
	}
	
	/* Configure server address */
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	
	/* Bind the socket */
	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	
	printf("Server listening on port 8080\n");
	
	/* Start listening for connections */
	if (listen(server_socket, 5) < 0)
	{
		perror("Listen failed");
		exit(EXIT_FAILURE);
	}

	/* Main loop to accept and handle clients */
	while (1)
	{
		client_socket = accept(server_socket, (struct sockaddr *)&server_addr, &addr_len);
		if (client_socket < 0)
		{
			perror("accept failed");
			continue; /* Continue to the next iteration on error */
		}
		
		printf("Client connected: %s\n", inet_ntoa(server_addr.sin_addr));
		
		/* Receive request */
		received_bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
		if (received_bytes > 0)
		{
			buffer[received_bytes] = '\0'; /* Null-terminate the received string */
			printf("Raw request: \"%s\"\n", buffer);
			
			/* Parse the request */
			sscanf(buffer, "%s %s %s", method, path, version);
			printf("Method: %s\nPath: %s\nVersion: %s\n", method, path, version);
		}

		/* Send response */
		send(client_socket, RESPONSE, sizeof(RESPONSE) - 1, 0);
		close(client_socket); /* Close the client connection */
	}

	close(server_socket); /* Close the server socket */
	return (0);
}
