// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

int main() {

    int sockfd, newsockfd;
    struct sockaddr_in server, client;
    socklen_t len;
    char buffer[100];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    bind(sockfd, (struct sockaddr*)&server, sizeof(server));
    listen(sockfd, 5);

    printf("Server waiting...\n");

    len = sizeof(client);
    newsockfd = accept(sockfd, (struct sockaddr*)&client, &len);

    recv(newsockfd, buffer, sizeof(buffer), 0);

    int pid = fork();

    if(pid == 0) {
        // CHILD PROCESS

        printf("\n--- CHILD PROCESS ---\n");
        printf("Child PID: %d\n", getpid());
        printf("Parent PID: %d\n", getppid());

        char temp[100], result[100];
        int j = 0;

        // Toggle case and extract characters only
        for(int i=0; buffer[i] != '\0'; i++) {
            if(isalpha(buffer[i])) {
                if(islower(buffer[i]))
                    temp[j++] = toupper(buffer[i]);
                else
                    temp[j++] = tolower(buffer[i]);
            }
        }

        temp[j] = '\0';

        // Reverse
        int len = strlen(temp);
        for(int i=0; i<len; i++)
            result[i] = temp[len - i - 1];

        result[len] = '\0';

        printf("Processed String (Child): %s\n", result);

        exit(0);
    }
    else {
        // PARENT PROCESS

        wait(NULL);

        printf("\n--- PARENT PROCESS ---\n");
        printf("Parent PID: %d\n", getpid());

        printf("Digits Processing:\n");

        for(int i=0; buffer[i] != '\0'; i++) {
            if(isdigit(buffer[i])) {
                int d = buffer[i] - '0';
                int result = d * (d + 1);
                printf("%d -> %d\n", d, result);
            }
        }
    }

    close(newsockfd);
    close(sockfd);

    return 0;
}
