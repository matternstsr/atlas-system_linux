#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define STAT_404 "HTTP/1.1 404 Not Found\r\n\r\n"
#define STAT_411 "HTTP/1.1 411 Length Required\r\n\r\n"
#define STAT_422 "HTTP/1.1 422 Unprocessable Entity\r\n\r\n"
#define STAT_201 "HTTP/1.1 201 Created\r\n"

typedef struct todo {
	size_t id;
	char *title;
	char *description;
	struct todo *next;
} todo_t;

todo_t *list = NULL;

void process_req(char *request, int fd);
void head_parser(char *query, int fd);
void task_parser(char *query, int fd);
void add_todo(char *desc, char *title, int fd);

int main(void)
{
	int socket_fd, connect;
	size_t bytes = 0;
	char buffer[4096];
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
		bytes = recv(connect, buffer, sizeof(buffer), 0);
		if (bytes > 0) {
			buffer[bytes] = '\0';
			printf("Raw request: \"%s\"\n", buffer);
			process_req(buffer, connect);
		}
		close(connect);
	}
	return 0;
}

void process_req(char *request, int fd)
{
	char meth[50], path[50];
	sscanf(request, "%s %s", meth, path);
	if (strcmp(meth, "POST") != 0 && strcmp(meth, "GET") != 0) {
		send(fd, STAT_404, strlen(STAT_404), 0);
		return;
	}
	if (strcmp(path, "/todos") != 0) {
		send(fd, STAT_404, strlen(STAT_404), 0);
		return;
	}
	head_parser(request, fd);
}

void head_parser(char *query, int fd)
{
	char *token = NULL, *lines[16] = {0}, *body = NULL;
	char key[50], val[50];
	int my_switch = 0, i = 0;

	while ((token = strsep(&query, "\r\n")) != NULL) {
		lines[i++] = token;
		if (i >= 16) break;
	}

	body = lines[i - 1];
	for (i = 1; lines[i]; i++) {
		sscanf(lines[i], "%[^:]: %s", key, val);
		if (strcmp(key, "Content-Length") == 0) {
			my_switch = 1;
		}
	}
	if (!my_switch) {
		send(fd, STAT_411, strlen(STAT_411), 0);
		return;
	}
	printf("%s\n", body);
	task_parser(body, fd);
}

void task_parser(char *query, int fd)
{
	char *token = NULL, *key_vals[16] = {0};
	char key[50], val[50], *title = NULL, *desc = NULL;
	int i = 0, my_switch = 0;

	while ((token = strsep(&query, "&")) != NULL) {
		if (token[0]) {
			key_vals[i++] = token;
			my_switch = 1;
		}
	}

	for (i = 0; key_vals[i]; i++) {
		sscanf(key_vals[i], "%[^=]=%s", key, val);
		if (strcmp(key, "title") == 0) {
			title = strdup(val);
		} else if (strcmp(key, "description") == 0) {
			desc = strdup(val);
		}
	}

	if (!title || !desc) {
		send(fd, STAT_422, strlen(STAT_422), 0);
		free(title);
		free(desc);
		return;
	}
	printf("title: %s\ndesc: %s\n", title, desc);
	add_todo(desc, title, fd);
	free(title);
	free(desc);
}

void add_todo(char *desc, char *title, int fd)
{
	static size_t id = 0;
	char buffer[1024];
	todo_t *new_todo = calloc(1, sizeof(todo_t));
	if (!new_todo) return;

	new_todo->id = id++;
	new_todo->title = strdup(title);
	new_todo->description = strdup(desc);
	new_todo->next = NULL;

	if (!list) {
		list = new_todo;
	} else {
		todo_t *tmp = list;
		while (tmp->next) {
			tmp = tmp->next;
		}
		tmp->next = new_todo;
	}

	snprintf(buffer, sizeof(buffer), "{\"id\":%lu,\"title\":\"%s\",\"description\":\"%s\"}",
			new_todo->id, title, desc);
	int len = strlen(buffer);
	printf("%s\n", buffer);
	dprintf(fd, "%s", STAT_201);
	dprintf(fd, "Content-Length: %d\r\n", len);
	dprintf(fd, "Content-Type: application/json\r\n\r\n");
	dprintf(fd, "%s", buffer);
}
