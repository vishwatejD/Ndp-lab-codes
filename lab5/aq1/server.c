#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define PORT 5000

int seats_route1 = 50;
int seats_route2 = 40;

int main() {

    int sockfd, newsockfd;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);
    char buffer[1024];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&server, sizeof(server));
    listen(sockfd, 5);

    printf("Travel Reservation Server Running...\n");

    while(1) {

        newsockfd = accept(sockfd, (struct sockaddr*)&client, &len);

        if(fork() == 0) {   // Child process

            close(sockfd);

            int route, seats_requested;

            // Send current seat status
            sprintf(buffer,
                "1. Manipal → Bangalore (Available: %d)\n"
                "2. Manipal → Mangalore (Available: %d)\n",
                seats_route1, seats_route2);

            send(newsockfd, buffer, strlen(buffer), 0);

            // Receive route & seats
            recv(newsockfd, &route, sizeof(route), 0);
            recv(newsockfd, &seats_requested, sizeof(seats_requested), 0);

            if(route == 1) {
                if(seats_requested <= seats_route1) {
                    seats_route1 -= seats_requested;
                    sprintf(buffer, "Booking Successful! Remaining seats: %d\n", seats_route1);
                } else {
                    sprintf(buffer, "Seats not available!\n");
                }
            }
            else if(route == 2) {
                if(seats_requested <= seats_route2) {
                    seats_route2 -= seats_requested;
                    sprintf(buffer, "Booking Successful! Remaining seats: %d\n", seats_route2);
                } else {
                    sprintf(buffer, "Seats not available!\n");
                }
            }
            else {
                sprintf(buffer, "Invalid Route!\n");
            }

            send(newsockfd, buffer, strlen(buffer), 0);

            close(newsockfd);
            exit(0);
        }

        close(newsockfd);
        wait(NULL);
    }

    close(sockfd);
    return 0;
}
