#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5000

int main() {

    int sockfd;
    struct sockaddr_in server;
    char buffer[1024];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&server, sizeof(server));

    // Receive seat info
    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("%s\n", buffer);

    int route, seats;

    printf("Enter route (1 or 2): ");
    scanf("%d", &route);

    printf("Enter number of seats: ");
    scanf("%d", &seats);

    send(sockfd, &route, sizeof(route), 0);
    send(sockfd, &seats, sizeof(seats), 0);

    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("%s\n", buffer);

    close(sockfd);
    return 0;
}
