#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define RESPONSE "HTTP/1.1 200 OK\r\n\r\n"

void parse_headers(char *request);

/**
 * main - Entry point of the server
 * Return: 0 on success
 */
int main(void)
{
    int server_fd, client_fd;
    size_t received_bytes = 0;
    char request_buffer[4096];
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("binding failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port 8080\n");
    fflush(stdout);

    if (listen(server_fd, 5) < 0)
    {
        perror("listening failed");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        client_fd = accept(server_fd, (struct sockaddr *)&server_addr, &addr_len);
        if (client_fd < 0)
        {
            perror("connection acceptance failed");
            exit(EXIT_FAILURE);
        }

        printf("Client connected: %s\n", inet_ntoa(server_addr.sin_addr));
        fflush(stdout);

        received_bytes = recv(client_fd, request_buffer, sizeof(request_buffer) - 1, 0);
        if (received_bytes > 0)
        {
            request_buffer[received_bytes] = '\0'; /* Null-terminate the buffer */
            printf("Raw request: \"%s\"\n", request_buffer);
            fflush(stdout);
            parse_headers(request_buffer); /* Use the new header parser */
        }

        send(client_fd, RESPONSE, sizeof(RESPONSE) - 1, 0);
        close(client_fd);
    }

    close(server_fd);
    return (0);
}

/**
 * parse_headers - Parses an HTTP request headers
 * @request: The request string to parse
 */
void parse_headers(char *request)
{
    int index = 0;
    char *line_token;
    char *header_lines[16] = {0}; /* Array to store header lines */
    char header_key[50], header_value[50];

    line_token = strtok(request, "\r\n");
    while (line_token)
    {
        header_lines[index++] = line_token; /* Store each line */
        line_token = strtok(NULL, "\r\n");
    }

    /* Process the headers starting from index 1 (skip request line) */
    for (index = 1; header_lines[index]; index++)
    {
        if (sscanf(header_lines[index], "%[^:]: %s", header_key, header_value) == 2)
        {
            printf("Header: \"%s\" -> \"%s\"\n", header_key, header_value);
        }
    }
}
