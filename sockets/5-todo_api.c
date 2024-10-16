#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define RESPONSE_OK "HTTP/1.1 200 OK\r\n"
#define RESPONSE_CREATED "HTTP/1.1 201 Created\r\n"
#define RESPONSE_NOT_FOUND "HTTP/1.1 404 Not Found\r\n\r\n"
#define RESPONSE_JSON_CONTENT "Content-Type: application/json\r\n"

typedef struct 
{
	int id;
	char title[100];
	char description[256];
} Todo;

Todo todos[100];
int todo_count = 0;

void send_todos(int conn)
{
	int i;
	char response[2048] = {0};
	char buffer[512];
	char final_response[2300];

	snprintf(response, sizeof(response), "%s%s", RESPONSE_OK, RESPONSE_JSON_CONTENT);
	strcat(response, "[");

	for (i = 0; i < todo_count; i++) 
	{
		if (i > 0) 
		{
			strcat(response, ",");
		}
		snprintf(buffer, sizeof(buffer), "{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}", 
				todos[i].id, todos[i].title, todos[i].description);
		strcat(response, buffer);
	}
	strcat(response, "]");

	size_t response_length = strlen(response);
	snprintf(final_response, sizeof(final_response), "%sContent-Length: %lu\r\n\r\n%s", 
			response, response_length, response);

	send(conn, final_response, strlen(final_response), 0);
}


void add_todo(char *title, char *description)
{
	if (todo_count < 100) 
	{
		todos[todo_count].id = todo_count;
		strncpy(todos[todo_count].title, title, sizeof(todos[todo_count].title) - 1);
		todos[todo_count].title[sizeof(todos[todo_count].title) - 1] = '\0'; /* Ensure null termination */
		strncpy(todos[todo_count].description, description, sizeof(todos[todo_count].description) - 1);
		todos[todo_count].description[sizeof(todos[todo_count].description) - 1] = '\0'; /* Ensure null termination */
		todo_count++;
	}
}

void parse_body(char *body, int conn)
{
	char response[512];
	char *title = NULL, *description = NULL, *token;

	token = strtok(body, "&");
	while (token) 
	{
		if (strncmp(token, "title=", 6) == 0) 
		{
			title = strdup(token + 6);
		} 
		else if (strncmp(token, "description=", 12) == 0) 
		{
			description = strdup(token + 12);
		}
		token = strtok(NULL, "&");
	}

	if (title && description) 
	{
		add_todo(title, description);
		snprintf(response, sizeof(response), "%sContent-Length: 51\r\nContent-Type: application/json\r\n\r\n{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}", 
				RESPONSE_CREATED, todo_count - 1, title, description);
		send(conn, response, strlen(response), 0);
	} 
	else 
	{
		send(conn, RESPONSE_NOT_FOUND, sizeof(RESPONSE_NOT_FOUND) - 1, 0);
	}

	free(title);
	free(description);
}

int main(void) 
{
	int socket_fd, new_con;
	size_t bytes = 0;
	char buffer[4096], meth[50], path[50], ver[50];
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1) 
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(8080);
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) 
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port 8080\n");
	fflush(stdout);

	if (listen(socket_fd, 5) < 0) 
	{
		perror("listen failed");
		exit(EXIT_FAILURE);
	}

	while (1) 
	{
		new_con = accept(socket_fd, (struct sockaddr *)&address, &addrlen);
		if (new_con < 0) 
		{
			perror("accept failed");
			exit(EXIT_FAILURE);
		}

		printf("Client connected: %s\n", inet_ntoa(address.sin_addr));
		fflush(stdout);

		bytes = recv(new_con, buffer, sizeof(buffer) - 1, 0);
		if (bytes > 0) 
		{
			buffer[bytes] = '\0';
			printf("Raw request: \"%s\"\n", buffer);
			fflush(stdout);
			sscanf(buffer, "%s %s %s", meth, path, ver);
			printf("Method: %s\nPath: %s\nVersion: %s\n", meth, path, ver);
			fflush(stdout);

			if (strcmp(meth, "GET") == 0 && strcmp(path, "/todos") == 0) 
			{
				send_todos(new_con);
			} 
			else if (strcmp(meth, "POST") == 0 && strcmp(path, "/todos") == 0) 
			{
				char *body = strstr(buffer, "\r\n\r\n") + 4;
				parse_body(body, new_con);
			} 
			else 
			{
				send(new_con, RESPONSE_NOT_FOUND, sizeof(RESPONSE_NOT_FOUND) - 1, 0);
			}
		}

		close(new_con);
	}

	close(socket_fd);
	return (0);
}
