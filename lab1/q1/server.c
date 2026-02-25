// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 100

void sortAscending(int arr[], int n) {
    for(int i=0;i<n-1;i++)
        for(int j=0;j<n-i-1;j++)
            if(arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
}

void sortDescending(int arr[], int n) {
    for(int i=0;i<n-1;i++)
        for(int j=0;j<n-i-1;j++)
            if(arr[j] < arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
}

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in server, client;
    int len, choice;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    bind(sockfd, (struct sockaddr*)&server, sizeof(server));
    listen(sockfd, 5);

    printf("Server waiting...\n");

    len = sizeof(client);
    newsockfd = accept(sockfd, (struct sockaddr*)&client, &len);

    while(1) {

        recv(newsockfd, &choice, sizeof(choice), 0);

        if(choice == 5) break;

        int n, arr[MAX];
        recv(newsockfd, &n, sizeof(n), 0);
        recv(newsockfd, arr, sizeof(int)*n, 0);

        if(choice == 1) {  // Search
            int key, found = 0;
            recv(newsockfd, &key, sizeof(key), 0);

            for(int i=0;i<n;i++)
                if(arr[i] == key)
                    found = 1;

            send(newsockfd, &found, sizeof(found), 0);
        }

        else if(choice == 2) {  // Sort Ascending
            sortAscending(arr, n);
            send(newsockfd, arr, sizeof(int)*n, 0);
        }

        else if(choice == 3) {  // Sort Descending
            sortDescending(arr, n);
            send(newsockfd, arr, sizeof(int)*n, 0);
        }

        else if(choice == 4) {  // Split Odd/Even
            int odd[MAX], even[MAX];
            int o=0, e=0;

            for(int i=0;i<n;i++) {
                if(arr[i] % 2 == 0)
                    even[e++] = arr[i];
                else
                    odd[o++] = arr[i];
            }

            send(newsockfd, &o, sizeof(o), 0);
            send(newsockfd, odd, sizeof(int)*o, 0);

            send(newsockfd, &e, sizeof(e), 0);
            send(newsockfd, even, sizeof(int)*e, 0);
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
