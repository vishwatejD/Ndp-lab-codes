// tcp_client.c
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 6000
#define SIZE 1024

int main() {
    int sockfd;
    char buffer[SIZE];
    struct sockaddr_in server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&server, sizeof(server));

    while (1) {
        printf("Enter string (Halt to exit): ");
        fgets(buffer, SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        write(sockfd, buffer, strlen(buffer)+1);

        if (strcmp(buffer, "Halt") == 0)
            break;

        memset(buffer, 0, SIZE);
        read(sockfd, buffer, SIZE);

        printf("\nServer Response:\n%s\n\n", buffer);
    }

    close(sockfd);
    return 0;
}
