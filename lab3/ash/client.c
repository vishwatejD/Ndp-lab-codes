// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {

    int sockfd;
    struct sockaddr_in server;
    char buffer[100];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&server, sizeof(server));

    printf("Enter a string: ");
    scanf(" %[^\n]", buffer);

    send(sockfd, buffer, sizeof(buffer), 0);

    close(sockfd);
    return 0;
}
