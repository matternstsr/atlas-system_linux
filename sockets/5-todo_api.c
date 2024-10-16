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
void send_response(int connect);

int main(void) {
    int socket_fd, connect;
    size_t bytes = 0;
    char buffer[BUFFER_SIZE];
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

    while (1) {
        connect = accept(socket_fd, (struct sockaddr *)&s_address, &addrlen);
        if (connect < 0)
            perror("accept failed"), exit(EXIT_FAILURE);

        printf("Client connected: %s\n", inet_ntoa(s_address.sin_addr));
        bytes = recv(connect, buffer, BUFFER_SIZE, 0);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            printf("Raw request: \"%s\"\n", buffer), fflush(stdout);
            body_parser(buffer);
            send_response(connect);
        }
        close(connect);
    }
    return 0;
}

void body_parser(char *query) {
    char *method = strtok(query, " ");

    if (strcmp(method, "POST") == 0) {
        char *body = strtok(NULL, "\r\n\r\n");
        query_parser(body);
    } else if (strcmp(method, "GET") == 0)
	{
        /* Handle GET requests? */
    }
}

void query_parser(char *query) {
    char key[50], val[100];
    sscanf(query, "title=%[^&]&description=%s", val, key);
    
    if (todo_count < MAX_TODOS) {
        todos[todo_count].id = todo_count;
        strcpy(todos[todo_count].title, val);
        strcpy(todos[todo_count].description, key);
        todo_count++;
        printf("Added todo: %s -> %s\n", val, key), fflush(stdout);
    }
}

void send_response(int connect) {
    char response[BUFFER_SIZE];
    int response_length = snprintf(response, sizeof(response),
                                    "HTTP/1.1 201 Created\r\n"
                                    "Content-Type: application/json\r\n"
                                    "Content-Length: %d\r\n"
                                    "\r\n"
                                    "{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
                                    (int)(strlen("{\"id\":0,\"title\":\"test\",\"description\":\"desc\"}") + 1),
                                    todos[todo_count - 1].id,
                                    todos[todo_count - 1].title,
                                    todos[todo_count - 1].description);
    send(connect, response, response_length, 0);
}
