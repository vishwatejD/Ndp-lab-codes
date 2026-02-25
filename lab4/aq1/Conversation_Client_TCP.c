// client.c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

struct Book {
    char title[50];
    char author[50];
    int acc_no;
    int pages;
    char publisher[50];
};

int main() {
    int sockfd, choice;
    struct sockaddr_in server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&server, sizeof(server));

    while (1) {
        printf("\n1.Insert\n2.Delete\n3.Display\n4.Search\n5.Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        send(sockfd, &choice, sizeof(choice), 0);

        if (choice == 1) {
            struct Book b;
            printf("Title: "); scanf(" %[^\n]", b.title);
            printf("Author: "); scanf(" %[^\n]", b.author);
            printf("Accession No: "); scanf("%d", &b.acc_no);
            printf("Pages: "); scanf("%d", &b.pages);
            printf("Publisher: "); scanf(" %[^\n]", b.publisher);

            send(sockfd, &b, sizeof(b), 0);

            char msg[50];
            recv(sockfd, msg, sizeof(msg), 0);
            printf("%s\n", msg);
        }

        else if (choice == 2) {
            int acc;
            printf("Enter accession number: ");
            scanf("%d", &acc);

            send(sockfd, &acc, sizeof(acc), 0);

            char msg[50];
            recv(sockfd, msg, sizeof(msg), 0);
            printf("%s\n", msg);
        }

        else if (choice == 3) {
            int count;
            recv(sockfd, &count, sizeof(count), 0);

            struct Book books[count];
            recv(sockfd, books, sizeof(books), 0);

            for (int i = 0; i < count; i++) {
                printf("\n%s | %s | %d | %d | %s\n",
                       books[i].title,
                       books[i].author,
                       books[i].acc_no,
                       books[i].pages,
                       books[i].publisher);
            }
        }

        else if (choice == 4) {
            char key[50];
            printf("Enter title or author: ");
            scanf(" %[^\n]", key);

            send(sockfd, key, sizeof(key), 0);

            while (1) {
                struct Book b;
                recv(sockfd, &b, sizeof(b), 0);

                if (b.acc_no == -1)
				printf("book or author not found");
                    break;

                printf("\n%s | %s | %d | %d | %s\n",
                       b.title, b.author,
                       b.acc_no, b.pages, b.publisher);
            }
        }

        else if (choice == 5) {
            break;
        }
    }

    close(sockfd);
    return 0;
}
