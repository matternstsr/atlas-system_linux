#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stddef.h>

typedef struct todo
{
	size_t id;
	char *title;
	char *description;
	struct todo *next;
} todo_t;

todo_t *todo_list = NULL;

void handle_request(char *request, int fd);
void parse_headers(char *query, int fd);
void parse_body(char *body, int fd);
void add_todo_item(char *description, char *title, int fd);

const char *STAT_404 = "HTTP/1.1 404 Not Found\r\n";
const char *STAT_411 = "HTTP/1.1 411 Length Required\r\n";
const char *STAT_422 = "HTTP/1.1 422 Unprocessable Entity\r\n";
const char *STAT_201 = "HTTP/1.1 201 Created\r\n";

int main(void)
{
	int server_socket, client_socket;
	size_t received_bytes = 0;
	char request_buffer[4096];
	struct sockaddr_in server_address;
	socklen_t address_length = sizeof(server_address);

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8080);
	server_address.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
	{
		perror("Binding failed");
		exit(EXIT_FAILURE);
	}

	printf("Server is now listening on port 8080\n");

	if (listen(server_socket, 5) < 0)
	{
		perror("Listening failed");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		client_socket = accept(server_socket, (struct sockaddr *)&server_address, &address_length);
		if (client_socket < 0)
		{
			perror("Connection acceptance failed");
			exit(EXIT_FAILURE);
		}

		printf("Client connected: %s\n", inet_ntoa(server_address.sin_addr));
		received_bytes = recv(client_socket, request_buffer, sizeof(request_buffer), 0);
		if (received_bytes > 0)
		{
			printf("Received request: \"%s\"\n", request_buffer);
			fflush(stdout);
			handle_request(request_buffer, client_socket);
		}
		close(client_socket);
	}
	return 0;
}

void handle_request(char *request, int fd)
{
	char method[50], path[50];

	printf("Processing request\n");
	sscanf(request, "%s %s", method, path);

	if (strcmp(method, "POST") != 0 && strcmp(method, "GET") != 0)
	{
		send(fd, STAT_404, sizeof(STAT_404), 0);
		return;
	}
	if (strcmp(path, "/todos") != 0)
	{
		send(fd, STAT_404, strlen(STAT_404), 0);
		return;
	}
	parse_headers(request, fd);
}

void parse_headers(char *query, int fd)
{
	int i = 0, has_content_length = 0;
	char *line = NULL, *header_lines[16] = {0}, *body = NULL, key[50], value[50];

	printf("Parsing headers\n");
	while ((line = strsep(&query, "\r\n")) != NULL && i < 16)
		header_lines[i++] = line;
	body = header_lines[i - 1];

	for (i = 1; header_lines[i]; i++)
	{
		sscanf(header_lines[i], "%[^:]: %s", key, value);
		if (strcmp(key, "Content-Length") == 0)
			has_content_length = 1;
	}
	if (!has_content_length)
	{
		send(fd, STAT_411, strlen(STAT_411), 0);
		return;
	}
	printf("Body: %s\n", body);
	parse_body(body, fd);
}

void parse_body(char *body, int fd)
{
	int i = 0, has_title = 0, has_description = 0;
	char *pair = NULL, *key_value_pairs[16] = {0}, key[50], value[50], *title = NULL, *description = NULL;

	printf("Parsing body\n");
	while ((pair = strsep(&body, "&")) != NULL && i < 16)
		key_value_pairs[i++] = pair;

	for (i = 0; key_value_pairs[i]; i++)
	{
		sscanf(key_value_pairs[i], "%[^=]=%s", key, value);
		if (strcmp(key, "title") == 0)
		{
			title = strdup(value);
			has_title = 1;
		} else if (strcmp(key, "description") == 0)
		{
			description = strdup(value);
			has_description = 1;
		}
	}

	if (!has_title || !has_description)
	{
		send(fd, STAT_422, strlen(STAT_422), 0);
		return;
	}

	printf("Title: %s\nDescription: %s\n", title, description);
	add_todo_item(description, title, fd);
}

void add_todo_item(char *description, char *title, int fd)
{
	static size_t todo_id = 0;
	char response_buffer[1024];
	int response_length;

	todo_t *new_todo = calloc(1, sizeof(todo_t));
	if (!new_todo) return;

	new_todo->id = todo_id++;
	new_todo->title = strdup(title);
	new_todo->description = strdup(description);
	new_todo->next = NULL;

	if (!todo_list)
		todo_list = new_todo;
	else
	{
		todo_t *current = todo_list;
		while (current->next)
			current = current->next;
		current->next = new_todo;
	}

	snprintf(response_buffer, sizeof(response_buffer), "{\"id\":%lu,\"title\":\"%s\",\"description\":\"%s\"}",
			new_todo->id, title, description);

	response_length = strlen(response_buffer);
	printf("%s\n", response_buffer);
	fflush(stdout);

	dprintf(fd, "%s", STAT_201);
	dprintf(fd, "Content-Length: %d\r\n", response_length);
	dprintf(fd, "Content-Type: application/json\r\n\r\n");
	dprintf(fd, "%s\r\n", response_buffer); /* added new line...*/
}

