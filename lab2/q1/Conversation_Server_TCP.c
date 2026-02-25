// TCP SERVER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SIZE 5000

int main()
{
    int sockfd, newsockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(sockfd, 5);

    printf("Server waiting...\n");

    len = sizeof(cliaddr);
    newsockfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);

    // ---- Receive Filename ----
    char filename[100];
    recv(newsockfd, filename, sizeof(filename), 0);

    FILE *fp = fopen(filename, "r+");

    if(fp == NULL)
    {
        char msg[] = "File not present";
        send(newsockfd, msg, sizeof(msg), 0);
        close(newsockfd);
        close(sockfd);
        return 0;
    }
    else
    {
        char msg[] = "File found";
        send(newsockfd, msg, sizeof(msg), 0);
    }

    while(1)
    {
        int option;
        recv(newsockfd, &option, sizeof(option), 0);

        // -------- SEARCH --------
        if(option == 1)
        {
            char search[100];
            recv(newsockfd, search, sizeof(search), 0);

            rewind(fp);
            char line[1000];
            int count = 0;

            while(fgets(line, sizeof(line), fp))
            {
                char *pos = line;
                while((pos = strstr(pos, search)) != NULL)
                {
                    count++;
                    pos += strlen(search);
                }
            }

            if(count > 0)
                send(newsockfd, &count, sizeof(count), 0);
            else
            {
                char msg[] = "String not found";
                send(newsockfd, msg, sizeof(msg), 0);
            }
        }

        // -------- REPLACE --------
        else if(option == 2)
        {
            char str1[100], str2[100];
            recv(newsockfd, str1, sizeof(str1), 0);
            recv(newsockfd, str2, sizeof(str2), 0);

            rewind(fp);

            char buffer[SIZE];
            int len = fread(buffer, 1, SIZE, fp);
            buffer[len] = '\0';

            char result[SIZE] = "";
            char *pos, *temp = buffer;
            int found = 0;

            while((pos = strstr(temp, str1)) != NULL)
            {
                found = 1;
                strncat(result, temp, pos - temp);
                strcat(result, str2);
                temp = pos + strlen(str1);
            }
            strcat(result, temp);

            if(found)
            {
                freopen(filename, "w", fp);
                fputs(result, fp);
                char msg[] = "String replaced";
                send(newsockfd, msg, sizeof(msg), 0);
            }
            else
            {
                char msg[] = "String not found";
                send(newsockfd, msg, sizeof(msg), 0);
            }
        }

        // -------- REORDER --------
        else if(option == 3)
        {
            rewind(fp);

            char data[SIZE];
            int len = fread(data, 1, SIZE, fp);

            for(int i=0;i<len-1;i++)
            {
                for(int j=i+1;j<len;j++)
                {
                    if(data[i] > data[j])
                    {
                        char t = data[i];
                        data[i] = data[j];
                        data[j] = t;
                    }
                }
            }

            freopen(filename, "w", fp);
            fwrite(data, 1, len, fp);

            char msg[] = "File reordered";
            send(newsockfd, msg, sizeof(msg), 0);
        }

        // -------- EXIT --------
        else if(option == 4)
        {
            break;
        }
    }

    fclose(fp);
    close(newsockfd);
    close(sockfd);

    return 0;
}
