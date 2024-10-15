#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

void handle_client(int client_sock)
{
	char buffer[BUFFER_SIZE];
	ssize_t bytes_received;
	const char *http_response;

	bytes_received = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
	if (bytes_received < 0)
	{
		perror("Receive failed");
		return;
	}
	buffer[bytes_received] = '\0'; /* Null-terminate the received string */

	/* Basic HTTP response */
	http_response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!";
	send(client_sock, http_response, strlen(http_response), 0);
}

int main(void)
{
	int sockfd, newsockfd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_len = sizeof(client_addr);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("Bind failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	if (listen(sockfd, 5) < 0)
	{
		perror("Listen failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port %d\n", PORT);

	while (1)
	{
		newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);
		if (newsockfd < 0)
		{
			perror("Accept failed");
			continue;
		}
		handle_client(newsockfd);
		close(newsockfd);
	}

	close(sockfd);
	return (0);
}
