#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define RESPONSE "HTTP/1.1 200 OK\r\n\r\n"

void parse_query(const char *query) {
	char *query_copy = strdup(query);
	char *pair = strtok(query_copy, "&");
	
	while (pair) {
		char *key = strtok(pair, "=");
		char *value = strtok(NULL, "=");
		if (key && value) {
			printf("Query: \"%s\" -> \"%s\"\n", key, value);
		}
		pair = strtok(NULL, "&");
	}
	
	free(query_copy);
}

int main(void) {
	int socket_fd, new_con;
	size_t bytes = 0;
	char buffer[4096], meth[50], path[50], ver[50];
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(8080);
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port 8080\n");
	fflush(stdout);

	if (listen(socket_fd, 5) < 0) {
		perror("listen failed");
		exit(EXIT_FAILURE);
	}

	while (1) {
		new_con = accept(socket_fd, (struct sockaddr *)&address, &addrlen);
		if (new_con < 0) {
			perror("accept failed");
			exit(EXIT_FAILURE);
		}

		printf("Client connected: %s\n", inet_ntoa(address.sin_addr));
		fflush(stdout);

		bytes = recv(new_con, buffer, sizeof(buffer) - 1, 0);
		if (bytes > 0) {
			buffer[bytes] = '\0';
			printf("Raw request: \"%s\"\n", buffer);
			fflush(stdout);
			sscanf(buffer, "%s %s %s", meth, path, ver);
			printf("Method: %s\nPath: %s\nVersion: %s\n", meth, path, ver);
			fflush(stdout);

			char *query_start = strchr(path, '?');
			if (query_start) {
				*query_start = '\0'; // Split path and query
				printf("Path: %s\n", path);
				parse_query(query_start + 1);
			} else {
				printf("Path: %s\n", path);
			}
		}

		send(new_con, RESPONSE, sizeof(RESPONSE) - 1, 0);
		close(new_con);
	}

	close(socket_fd);
	return (0);
}
