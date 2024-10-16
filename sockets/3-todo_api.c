#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define HTTP_RESPONSE "HTTP/1.1 200 OK\r\n\r\n"
#define BUFFER_SIZE 4096
#define PATH_SIZE 50

void parse_http_headers(char *request);
void parse_http_body(char *request);

int main(void)
{
	int server_socket, client_socket;
	size_t received_bytes = 0;
	char request_buffer[BUFFER_SIZE];
	char request_path[PATH_SIZE];
	struct sockaddr_in server_address;
	socklen_t address_length = sizeof(server_address);

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8080);
	server_address.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
	{
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port 8080\n");
	if (listen(server_socket, 5) < 0)
	{
		perror("Listen failed");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		client_socket = accept(server_socket, (struct sockaddr *)&server_address, &address_length);
		if (client_socket < 0)
		{
			perror("Accept failed");
			exit(EXIT_FAILURE);
		}

		printf("Client connected: %s\n", inet_ntoa(server_address.sin_addr));
		received_bytes = recv(client_socket, request_buffer, sizeof(request_buffer) - 1, 0);
		if (received_bytes > 0)
		{
			request_buffer[received_bytes] = '\0';
			printf("Raw request: \"%s\"\n", request_buffer);
			sscanf(request_buffer, "%*s %s", request_path);
			printf("Path: %s\n", request_path);
			parse_http_body(request_buffer);
		}

		send(client_socket, HTTP_RESPONSE, strlen(HTTP_RESPONSE), 0);
		close(client_socket);
	}

	close(server_socket);
	return 0;
}

void parse_http_body(char *request)
{
	char *header_lines[16] = {0};
	char *body = NULL, *body_start, *line;
	int i = 0;

	line = strtok(request, "\r\n");
	while (line && i < 16)
	{
		header_lines[i++] = line;
		line = strtok(NULL, "\r\n");
	}

	body = header_lines[i - 1];
	if (body && strstr(body, "Content-Length:"))
	{
		body_start = strstr(request, "\r\n\r\n") + 4;
		parse_http_parameters(body_start);
	}
}

void parse_http_parameters(char *body)
{
	char *key_value_pairs[16] = {0};
	char key[50], value[50], *pair;
	int i = 0;

	pair = strtok(body, "&");
	while (pair && i < 16)
	{
		key_value_pairs[i++] = pair;
		pair = strtok(NULL, "&");
	}

	for (int j = 0; j < i; j++)
	{
		sscanf(key_value_pairs[j], "%[^=]=%s", key, value);
		printf("Body param: \"%s\" -> \"%s\"\n", key, value);
	}
}
