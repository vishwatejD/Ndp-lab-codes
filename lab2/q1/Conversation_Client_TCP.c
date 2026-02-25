// TCP CLIENT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    // ---- Send filename ----
    char filename[100];
    printf("Enter filename: ");
    scanf("%s", filename);

    send(sockfd, filename, sizeof(filename), 0);

    char response[100];
    recv(sockfd, response, sizeof(response), 0);

    printf("%s\n", response);

    if(strcmp(response, "File not present") == 0)
    {
        close(sockfd);
        return 0;
    }

    while(1)
    {
        int option;

        printf("\n1.Search\n2.Replace\n3.Reorder\n4.Exit\n");
        printf("Enter option: ");
        scanf("%d", &option);

        send(sockfd, &option, sizeof(option), 0);

        if(option == 1)
        {
            char search[100];
            printf("Enter string to search: ");
            scanf("%s", search);
            send(sockfd, search, sizeof(search), 0);

            int count;
            if(recv(sockfd, &count, sizeof(count), 0) > 0)
                printf("String found %d times\n", count);
            else
                printf("String not found\n");
        }

        else if(option == 2)
        {
            char str1[100], str2[100];

            printf("Enter string to replace: ");
            scanf("%s", str1);

            printf("Enter new string: ");
            scanf("%s", str2);

            send(sockfd, str1, sizeof(str1), 0);
            send(sockfd, str2, sizeof(str2), 0);

            recv(sockfd, response, sizeof(response), 0);
            printf("%s\n", response);
        }

        else if(option == 3)
        {
            recv(sockfd, response, sizeof(response), 0);
            printf("%s\n", response);
        }

        else if(option == 4)
        {
            break;
        }
    }

    close(sockfd);
    return 0;
}
