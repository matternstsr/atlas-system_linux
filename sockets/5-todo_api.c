#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
#define MAX_TODOS 100

typedef struct 
{
	int id;
	char title[100];
	char description[256];
} Todo;

Todo todos[MAX_TODOS];
int todo_count = 0;

void send_response(int conn, const char *status, const char *content_type, const char *body) 
{
	char response[BUFFER_SIZE];
	snprintf(response, sizeof(response), "%sContent-Type: %s\r\nContent-Length: %lu\r\n\r\n%s",
			status, content_type, strlen(body), body);
	send(conn, response, strlen(response), 0);
}

void handle_get(int conn) 
{
	char body[BUFFER_SIZE];
	int i;
	body[0] = '\0';

	strcat(body, "[");
	for (i = 0; i < todo_count; i++) 
	{
		if (i > 0) 
		{
			strcat(body, ",");
		}
		snprintf(body + strlen(body), sizeof(body) - strlen(body), "{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
				todos[i].id, todos[i].title, todos[i].description);
	}
	strcat(body, "]");
	send_response(conn, "HTTP/1.1 200 OK\r\n", "application/json", body);
}

void handle_post(int conn, const char *body) 
{
	char title[100] = {0}, description[256] = {0};
	char response_body[256];

	sscanf(body, "title=%99[^&]&description=%255s", title, description);

	if (todo_count < MAX_TODOS) 
	{
		todos[todo_count].id = todo_count;
		strncpy(todos[todo_count].title, title, sizeof(todos[todo_count].title) - 1);
		strncpy(todos[todo_count].description, description, sizeof(todos[todo_count].description) - 1);
		todo_count++;

		snprintf(response_body, sizeof(response_body), "{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
				todo_count - 1, title, description);
		send_response(conn, "HTTP/1.1 201 Created\r\n", "application/json", response_body);
	} 
	else 
	{
		send_response(conn, "HTTP/1.1 500 Internal Server Error\r\n", "text/plain", "Todo list full");
	}
}

int main(void) 
{
	int socket_fd, connect;
	char buffer[BUFFER_SIZE];
	char method[10], path[50];
	struct sockaddr_in s_address;
	socklen_t addrlen = sizeof(s_address);

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1) 
	{
		perror("socket failed"), exit(EXIT_FAILURE);
	}

	s_address.sin_family = AF_INET;
	s_address.sin_port = htons(8080);
	s_address.sin_addr.s_addr = INADDR_ANY;

	if (bind(socket_fd, (struct sockaddr *)&s_address, sizeof(s_address)) < 0) 
	{
		perror("bind failed"), exit(EXIT_FAILURE);
	}

	printf("Server listening on port 8080\n");
	if (listen(socket_fd, 5) < 0) 
	{
		perror("listen failed"), exit(EXIT_FAILURE);
	}

	while (1) 
	{
		connect = accept(socket_fd, (struct sockaddr *)&s_address, &addrlen);
		if (connect < 0) 
		{
			perror("accept failed"), exit(EXIT_FAILURE);
		}

		printf("Client connected: %s\n", inet_ntoa(s_address.sin_addr));

		ssize_t bytes = recv(connect, buffer, sizeof(buffer) - 1, 0);
		if (bytes > 0) 
		{
			buffer[bytes] = '\0';
			printf("Raw request: \"%s\"\n", buffer);
			sscanf(buffer, "%s %s", method, path);
			char *body = strstr(buffer, "\r\n\r\n");
			body = body ? body + 4 : NULL;

			if (strcmp(method, "GET") == 0 && strcmp(path, "/todos") == 0) 
			{
				handle_get(connect);
			} 
			else if (strcmp(method, "POST") == 0 && strcmp(path, "/todos") == 0 && body) 
			{
				handle_post(connect, body);
			} 
			else 
			{
				send_response(connect, "HTTP/1.1 404 Not Found\r\n", "text/plain", "Not Found");
			}
		}

		close(connect);
	}

	close(socket_fd);
	return 0;
}
