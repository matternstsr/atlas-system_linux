#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
#define MAX_TODOS 100

typedef struct {
	int id;
	char title[50];
	char description[100];
} Todo;

Todo todos[MAX_TODOS];
int todo_count = 0;

void query_parser(char *query, int connect, const char *method);
void body_parser(char *query, int connect, const char *method);
void send_response(int connect, int status_code, const char *body);

int main(void)
{
	int socket_fd, connect;
	size_t bytes = 0;
	char buffer[BUFFER_SIZE], path[50], method[10];
	struct sockaddr_in s_address;
	socklen_t addrlen = sizeof(s_address);

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
		perror("socket failed"), exit(EXIT_FAILURE);
	
	s_address.sin_family = AF_INET;
	s_address.sin_port = htons(8080);
	s_address.sin_addr.s_addr = INADDR_ANY;

	if (bind(socket_fd, (struct sockaddr *)&s_address, sizeof(s_address)) < 0)
		perror("bind failed"), exit(EXIT_FAILURE);
	
	printf("Server listening on port 8080\n");
	
	if (listen(socket_fd, 5) < 0)
		perror("listen failed"), exit(EXIT_FAILURE);
	
	while (1)
	{
		connect = accept(socket_fd, (struct sockaddr *)&s_address, &addrlen);
		if (connect < 0)
			perror("accept failed"), exit(EXIT_FAILURE);
		
		printf("Client connected: %s\n", inet_ntoa(s_address.sin_addr));
		bytes = recv(connect, buffer, sizeof(buffer) - 1, 0);
		if (bytes > 0)
		{
			buffer[bytes] = '\0';
			printf("Raw request: \"%s\"\n", buffer), fflush(stdout);
			sscanf(buffer, "%s %s", method, path);
			body_parser(buffer, connect, method);
		}
		close(connect);
	}
	
	return (0);
}

void body_parser(char *query, int connect, const char *method)
{
	int i = 0, my_switch = 0;
	char *token = NULL, *lines[16] = {0}, *body = NULL;

	do {
		token = strsep(&query, "\r\n");
		if (token)
			lines[i++] = token, my_switch = 1;
	} while (token && my_switch--);

	body = lines[i - 1];
	query_parser(body, connect, method);
}

void query_parser(char *query, int connect, const char *method)
{
	char response_body[1024];
	int i;

	if (strcmp(method, "GET") == 0) {
		if (strcmp(query, "/todos") == 0) {
			snprintf(response_body, sizeof(response_body), "[");
			for (i = 0; i < todo_count; i++) {
				char todo_item[256];
				snprintf(todo_item, sizeof(todo_item), "{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}%s",
						todos[i].id, todos[i].title, todos[i].description, (i < todo_count - 1) ? "," : "");
				strncat(response_body, todo_item, sizeof(response_body) - strlen(response_body) - 1);
			}
			strncat(response_body, "]", sizeof(response_body) - strlen(response_body) - 1);
			send_response(connect, 200, response_body);
		} else {
			send_response(connect, 404, "Not Found");
		}
	} else if (strcmp(method, "POST") == 0) {
		if (todo_count < MAX_TODOS) {
			sscanf(query, "title=%[^&]&description=%s", todos[todo_count].title, todos[todo_count].description);
			todos[todo_count].id = todo_count;
			todo_count++;
			send_response(connect, 201, "{\"status\":\"Created\"}");
		} else {
			send_response(connect, 500, "Server full");
		}
	} else {
		send_response(connect, 405, "Method Not Allowed");
	}
}

void send_response(int connect, int status_code, const char *body)
{
	char header[256];
	int body_length = strlen(body);
	
	snprintf(header, sizeof(header), "HTTP/1.1 %d %s\r\nContent-Type: application/json\r\nContent-Length: %d\r\n\r\n%s",
			status_code, status_code == 200 ? "OK" : 
			status_code == 201 ? "Created" : 
			status_code == 404 ? "Not Found" : "Error", body_length, body);
	
	send(connect, header, strlen(header), 0);
}
