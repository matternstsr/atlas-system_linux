#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

void query_parser(char *query, int connect);
void body_parser(char *query, int connect);
void send_response(int connect, int status_code, const char *body);

int main(void)
{
	int socket_fd, connect;
	size_t bytes = 0;
	char buffer[BUFFER_SIZE], path[50];
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
			sscanf(buffer, "%*s %s", path);
			printf("Path: %s\n", path), fflush(stdout);
			body_parser(buffer, connect);
		}
		close(connect);
	}
	
	return (0);
}

void body_parser(char *query, int connect)
{
	int i = 0, my_switch = 0;
	char *token = NULL, *lines[16] = {0}, *body = NULL;

	do {
		token = strsep(&query, "\r\n");
		if (token)
			lines[i++] = token, my_switch = 1;
	} while (token && my_switch--);

	body = lines[i - 1];
	query_parser(body, connect);
}

void query_parser(char *query, int connect)
{
	int i = 0, my_switch = 0;
	char *token = NULL, *key_vals[16] = {0}, key[50], val[50];
	char response_body[256];

	do {
		token = strsep(&query, "&");
		if (token && token[0])
			key_vals[i++] = token, my_switch = 1;
	} while (token && my_switch--);

	for (i = 0; key_vals[i]; i++)
	{
		sscanf(key_vals[i], "%[^=]=%s", key, val);
		printf("Body param: \"%s\" -> \"%s\"\n", key, val), fflush(stdout);
	}

	if (strcmp(query, "/todos") == 0) {
		snprintf(response_body, sizeof(response_body), "{\"id\":0,\"title\":\"Holberton\",\"description\":\"School\"}");
		send_response(connect, 200, response_body);
	} else {
		send_response(connect, 404, "Not Found");
	}
}

void send_response(int connect, int status_code, const char *body)
{
	char header[256];
	int body_length = strlen(body);
	
	snprintf(header, sizeof(header), "HTTP/1.1 %d %s\r\nContent-Type: application/json\r\nContent-Length: %d\r\n\r\n%s",
			status_code, status_code == 200 ? "OK" : "Not Found", body_length, body);
	
	send(connect, header, strlen(header), 0);
}
