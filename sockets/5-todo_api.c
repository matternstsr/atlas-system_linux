#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_TODOS 100

typedef struct {
	int id;
	char title[100];
	char description[256];
} Todo;

Todo todos[MAX_TODOS];
int todo_count = 0;

void add_todo(const char *title, const char *description) {
	if (todo_count < MAX_TODOS) {
		todos[todo_count].id = todo_count;
		strncpy(todos[todo_count].title, title, sizeof(todos[todo_count].title) - 1);
		strncpy(todos[todo_count].description, description, sizeof(todos[todo_count].description) - 1);
		todo_count++;
	}
}

void send_todos(int conn) {
	char response[2048];
	snprintf(response, sizeof(response),
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: application/json\r\n"
			"Content-Length: %lu\r\n"
			"\r\n"
			"[", (unsigned long)(2 + 3 * todo_count)); // Adjust content length

	for (int i = 0; i < todo_count; i++) {
		if (i > 0) {
			strcat(response, ",");
		}
		char buffer[256];
		snprintf(buffer, sizeof(buffer), "{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
				todos[i].id, todos[i].title, todos[i].description);
		strcat(response, buffer);
	}
	strcat(response, "]");

	send(conn, response, strlen(response), 0);
}

void handle_post(int conn, char *body) {
	char title[100], description[256];
	if (sscanf(body, "title=%99[^&]&description=%255s", title, description) == 2) {
		add_todo(title, description);
		char response_body[256];
		snprintf(response_body, sizeof(response_body), "{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
				todo_count - 1, title, description);

		char response[512];
		snprintf(response, sizeof(response),
				"HTTP/1.1 201 Created\r\n"
				"Content-Type: application/json\r\n"
				"Content-Length: %lu\r\n"
				"\r\n"
				"%s", (unsigned long)strlen(response_body), response_body);
		send(conn, response, strlen(response), 0);
	} else {
		send(conn, "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 9\r\n\r\nNot Found", 85, 0);
	}
}

void handle_request(int conn) {
	char buffer[1024];
	ssize_t bytes = recv(conn, buffer, sizeof(buffer) - 1, 0);
	if (bytes < 0) {
		perror("recv failed");
		close(conn);
		return;
	}
	buffer[bytes] = '\0';

	if (strstr(buffer, "GET /todos")) {
		send_todos(conn);
	} else if (strstr(buffer, "POST /todos")) {
		char *body = strstr(buffer, "\r\n\r\n");
		if (body) {
			handle_post(conn, body + 4);
		}
	} else {
		send(conn, "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 9\r\n\r\nNot Found", 85, 0);
	}
	close(conn);
}

int main() {
	int server_fd, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	while (1) {
		new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
		if (new_socket < 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}
		handle_request(new_socket);
	}

	return 0;
}
