#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h> // Include for isspace

#define RESPONSE "HTTP/1.1 200 OK\r\n\r\n"

void parse_headers(char *request);

int main(void)
{
	int server_fd, client_fd;
	size_t received_bytes = 0;
	char request_buffer[4096];
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_len = sizeof(client_addr);

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
	{
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("binding failed");
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port 8080\n");
	fflush(stdout);

	if (listen(server_fd, 5) < 0)
	{
		perror("listening failed");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
		if (client_fd < 0)
		{
			perror("connection acceptance failed");
			exit(EXIT_FAILURE);
		}

		printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));
		fflush(stdout);

		received_bytes = recv(client_fd, request_buffer, sizeof(request_buffer) - 1, 0);
		if (received_bytes > 0)
		{
			request_buffer[received_bytes] = '\0'; /* Null-terminate the buffer */
			printf("Raw request: \"%s\"\n", request_buffer);
			fflush(stdout);
			parse_headers(request_buffer); /* Use the new header parser */
		}

		send(client_fd, RESPONSE, strlen(RESPONSE), 0); /* Correct response length */
		close(client_fd);
	}

	close(server_fd);
	return 0;
}

void parse_headers(char *request)
{
	char *line_token, *key_start, *value_start;
	char header_key[256], header_value[256];

	line_token = strtok(request, "\r\n");
	while (line_token)
	{
		if (line_token[0] != '\0' && line_token != request) 
		{
			if (sscanf(line_token, "%255[^:]: %255[^\r\n]", header_key, header_value) == 2)
			{
				// Trim whitespace from key
				key_start = header_key;
				while (isspace(*key_start)) key_start++;
				
				// Trim whitespace from value
				value_start = header_value;
				while (isspace(*value_start)) value_start++;
				
				printf("Header: \"%s\" -> \"%s\"\n", key_start, value_start);
			}
		}
		line_token = strtok(NULL, "\r\n");
	}
}
