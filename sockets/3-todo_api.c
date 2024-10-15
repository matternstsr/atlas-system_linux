#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

void send_file_contents(int client_sock, const char *filename)
{
	FILE *file = fopen(filename, "r");
	char buffer[BUFFER_SIZE];

	if (file == NULL)
	{
		perror("File opening failed");
		return;
	}

	while (fgets(buffer, sizeof(buffer), file) != NULL)
	{
		send(client_sock, buffer, strlen(buffer), 0);
	}

	fclose(file);
}

int main(void)
{
	int sockfd, newsockfd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_len = sizeof(client_addr);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("Bind failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	if (listen(sockfd, 5) < 0)
	{
		perror("Listen failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port %d\n", PORT);

	while (1)
	{
		newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);
		if (newsockfd < 0)
		{
			perror("Accept failed");
			continue;
		}
		send_file_contents(newsockfd, "example.txt"); /* Example file */
		close(newsockfd);
	}

	close(sockfd);
	return (0);
}
