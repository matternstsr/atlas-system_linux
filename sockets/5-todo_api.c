#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_TODOS 100
#define BUFFER_SIZE 4096

typedef struct {
	int id;
	char title[50];
	char description[100];
} Todo;

Todo todos[MAX_TODOS];
int todo_count = 0;

void query_parser(char *query);
void body_parser(char *query);
void send_response(int connect, int status_code, const char *body);

int main(void) {
	int socket_fd, connect;
	size_t bytes = 0;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in s_address;
	socklen_t addrlen = sizeof(s_address);

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	s_address.sin_family = AF_INET;
	s_address.sin_port = htons(8080);
	s_address.sin_addr.s_addr = INADDR_ANY;

	if (bind(socket_fd, (struct sockaddr *)&s_address, sizeof(s_address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port 8080\n");
	if (listen(socket_fd, 5) < 0) {
		perror("listen failed");
		exit(EXIT_FAILURE);
	}

	while (1) {
		connect = accept(socket_fd, (struct sockaddr *)&s_address, &addrlen);
		if (connect < 0) {
			perror("accept failed");
			exit(EXIT_FAILURE);
		}

		printf("Client connected: %s\n", inet_ntoa(s_address.sin_addr));
		bytes = recv(connect, buffer, BUFFER_SIZE - 1, 0);
		if (bytes > 0) {
			buffer[bytes] = '\0';
			printf("Raw request: \"%s\"\n", buffer);
			body_parser(buffer);
		}
		close(connect);
	}
	return 0;
}

void body_parser(char *query) {
	char *method = strtok(query, " ");

	if (strcmp(method, "POST") == 0) {
		char *body = strstr(query, "\r\n\r\n");
		if (body) {
			body += 4;
			query_parser(body);
			send_response(connect, 201, "{\"id\":0,\"title\":\"New Todo\",\"description\":\"\"}");
		}
	} else if (strcmp(method, "GET") == 0) {
		if (strcmp(path, "/todos") == 0) {
			char response_body[BUFFER_SIZE] = "[";
			for (int i = 0; i < todo_count; i++) {
				char todo_json[256];
				snprintf(todo_json, sizeof(todo_json), "{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}%s",
						todos[i].id, todos[i].title, todos[i].description, (i < todo_count - 1) ? "," : "");
				strcat(response_body, todo_json);
			}
			strcat(response_body, "]");
			send_response(connect, 200, response_body);
		} else {
			send_response(connect, 404, "Not Found");
		}
	}
}

void query_parser(char *query) {
	char title[50] = "";
	char description[100] = "";

	sscanf(query, "title=%49[^&]&description=%99s", title, description);

	if (todo_count < MAX_TODOS) {
		todos[todo_count].id = todo_count;
		strcpy(todos[todo_count].title, title);
		strcpy(todos[todo_count].description, description);
		todo_count++;
		printf("Added todo: %s -> %s\n", title, description);
	}
}

void send_response(int connect, int status_code, const char *body) {
	char response[BUFFER_SIZE];
	int content_length = strlen(body);
	const char *status_message = (status_code == 200) ? "OK" : "Created";

	snprintf(response, sizeof(response),
			"HTTP/1.1 %d %s\r\n"
			"Content-Type: application/json\r\n"
			"Content-Length: %d\r\n"
			"\r\n"
			"%s", status_code, status_message, content_length, body);
	send(connect, response, strlen(response), 0);
}
