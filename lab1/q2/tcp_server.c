// tcp_server.c
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 6000
#define SIZE 1024

int isPalindrome(char str[]) {
    int l = 0;
    int r = strlen(str) - 1;
    while (l < r) {
        if (str[l] != str[r])
            return 0;
        l++;
        r--;
    }
    return 1;
}

void countVowels(char str[], int *a,int *e,int *i,int *o,int *u) {
    for (int j = 0; str[j]; j++) {
        char ch = tolower(str[j]);
        if (ch=='a') (*a)++;
        else if (ch=='e') (*e)++;
        else if (ch=='i') (*i)++;
        else if (ch=='o') (*o)++;
        else if (ch=='u') (*u)++;
    }
}

int main() {
    int server_fd, new_socket;
    char buffer[SIZE], result[SIZE];
    struct sockaddr_in server;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&server, sizeof(server));
    listen(server_fd, 5);

    new_socket = accept(server_fd, NULL, NULL);

    while (1) {
        memset(buffer, 0, SIZE);
        read(new_socket, buffer, SIZE);

        if (strcmp(buffer, "Halt") == 0)
            break;

        int length = strlen(buffer);
        int a=0,e=0,i=0,o=0,u=0;

        int palindrome = isPalindrome(buffer);
        countVowels(buffer, &a,&e,&i,&o,&u);

        sprintf(result,
                "Palindrome: %s\nLength: %d\nVowels -> a:%d e:%d i:%d o:%d u:%d",
                palindrome ? "Yes" : "No",
                length, a,e,i,o,u);

        write(new_socket, result, strlen(result)+1);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
