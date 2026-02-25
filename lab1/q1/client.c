// client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 100

int main() {

    int sockfd, choice;
    struct sockaddr_in server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&server, sizeof(server));

    while(1) {

        printf("\n1.Search\n2.Sort Ascending\n3.Sort Descending\n4.Split Odd/Even\n5.Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        send(sockfd, &choice, sizeof(choice), 0);

        if(choice == 5) break;

        int n, arr[MAX];

        printf("Enter number of elements: ");
        scanf("%d", &n);

        printf("Enter elements:\n");
        for(int i=0;i<n;i++)
            scanf("%d", &arr[i]);

        send(sockfd, &n, sizeof(n), 0);
        send(sockfd, arr, sizeof(int)*n, 0);

        if(choice == 1) {
            int key, found;
            printf("Enter number to search: ");
            scanf("%d", &key);

            send(sockfd, &key, sizeof(key), 0);
            recv(sockfd, &found, sizeof(found), 0);

            if(found)
                printf("Number found!\n");
            else
                printf("Number not found!\n");
        }

        else if(choice == 2 || choice == 3) {
            recv(sockfd, arr, sizeof(int)*n, 0);
            printf("Result:\n");
            for(int i=0;i<n;i++)
                printf("%d ", arr[i]);
            printf("\n");
        }

        else if(choice == 4) {
            int o, e, odd[MAX], even[MAX];

            recv(sockfd, &o, sizeof(o), 0);
            recv(sockfd, odd, sizeof(int)*o, 0);

            recv(sockfd, &e, sizeof(e), 0);
            recv(sockfd, even, sizeof(int)*e, 0);

            printf("Odd numbers: ");
            for(int i=0;i<o;i++)
                printf("%d ", odd[i]);

            printf("\nEven numbers: ");
            for(int i=0;i<e;i++)
                printf("%d ", even[i]);

            printf("\n");
        }
    }

    close(sockfd);
    return 0;
}
