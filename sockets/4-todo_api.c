	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>

	#define RESPONSE_OK "HTTP/1.1 200 OK\r\n\r\n"
	#define RESPONSE_CREATED "HTTP/1.1 201 Created\r\nContent-Type: application/json\r\n\r\n"
	#define RESPONSE_LENGTH_REQUIRED "HTTP/1.1 411 Length Required\r\n\r\n"
	#define RESPONSE_UNPROCESSABLE_ENTITY "HTTP/1.1 422 Unprocessable Entity\r\n\r\n"
	#define RESPONSE_NOT_FOUND "HTTP/1.1 404 Not Found\r\n\r\n"

	#define MAX_TODOS 100

	typedef struct
	{
		int id;
		char title[100];
		char description[256];
	} Todo;

	static Todo todos[MAX_TODOS];
	static int todo_count = 0;

	void parse_body(char *body, char *title, char *description);
	int create_todo(char *title, char *description);
	char *process_request(char *method, char *path, char *body);
	char *make_response(char *buffer);

	int main(void)
	{
		int socket_fd, new_conn;
		size_t bytes = 0;
		char buffer[4096], *response;
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
			new_conn = accept(socket_fd, (struct sockaddr *)&address, &addrlen);
			if (new_conn < 0)
			{
				perror("accept failed");
				exit(EXIT_FAILURE);
			}

			printf("Client connected: %s\n", inet_ntoa(address.sin_addr));
			fflush(stdout);

			bytes = recv(new_conn, buffer, sizeof(buffer) - 1, 0);
			if (bytes > 0)
			{
				buffer[bytes] = '\0';
				printf("Raw request: \"%s\"\n", buffer);
				fflush(stdout);
				response = make_response(buffer);
				send(new_conn, response, strlen(response), 0);
				free(response);
			}

			close(new_conn);
		}

		close(socket_fd);
		return 0;
	}

	void parse_body(char *body, char *title, char *description)
	{
		char *title_param = strstr(body, "title=");
		char *description_param = strstr(body, "description=");

		if (title_param)
			sscanf(title_param + 6, "%[^&]", title);
		if (description_param)
			sscanf(description_param + 12, "%[^&]", description);
	}

	int create_todo(char *title, char *description)
	{
		if (todo_count < MAX_TODOS)
		{
			todos[todo_count].id = todo_count;
			strncpy(todos[todo_count].title, title, sizeof(todos[todo_count].title) - 1);
			strncpy(todos[todo_count].description, description, sizeof(todos[todo_count].description) - 1);
			return todo_count++;
		}
		return -1;
	}

	char *process_request(char *method, char *path, char *body)
	{
		char response[512];
		char json_response[256];
		int content_length;

		if (strcmp(method, "POST") == 0 && strcmp(path, "/todos") == 0)
		{
			char title[100] = {0}, description[256] = {0};
			parse_body(body, title, description);

			if (strlen(title) > 0 && strlen(description) > 0)
			{
				int id = create_todo(title, description);
				if (id != -1)
				{
					snprintf(json_response, sizeof(json_response), 
							"{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}", 
							id, title, description);
					
					content_length = strlen(json_response);
					
					snprintf(response, sizeof(response), 
							"HTTP/1.1 201 Created\r\nContent-Length: %d\r\nContent-Type: application/json\r\n\r\n%s",
							content_length, json_response);
					return strdup(response);
				}
			}
			else
				return strdup(RESPONSE_UNPROCESSABLE_ENTITY);
		}

		return strdup(RESPONSE_NOT_FOUND);
	}


	char *make_response(char *buffer)
	{
		char method[10], path[100], version[10];
		char *body = strstr(buffer, "\r\n\r\n") + 4;

		sscanf(buffer, "%s %s %s", method, path, version);
		return process_request(method, path, body);
	}
