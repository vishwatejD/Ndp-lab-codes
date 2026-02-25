#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#define MAXSIZE 100

struct Book{
	char title[50];
	char author[50];
	int acc_no;
	int pages;
	int publisher[50];
};

struct Book books[MAXSIZE];
int count=0;

int main()
{
	int sockfd,newsockfd,retval,i;
	socklen_t actuallen;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr,clientaddr;
	char buff[MAXSIZE];
	int choice=0;
	
	int a=0;
	sockfd=socket(AF_INET,SOCK_STREAM,0);

	if(sockfd==-1)
	{
	printf("\nSocket creation error");
	}

	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(8080);
	serveraddr.sin_addr.s_addr=htons(INADDR_ANY);
	retval=bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(retval==1)
	{
		printf("Binding error");
		close(sockfd);
	}

	retval=listen(sockfd,1);
	if(retval==-1)
	{
	close(sockfd);
	}
	actuallen=sizeof(clientaddr);
	newsockfd=accept(sockfd,(struct sockaddr*)&clientaddr,&actuallen);
	if(newsockfd==-1)
	{
		close(sockfd);
	}

	while(1){
		recv(newsockfd, &choice, sizeof(choice),0);

		if(choice == 1){//insert
			recv(newsockfd,&books[count],sizeof(struct Book),0);
			count++;
			send(newsockfd,"Book Inserted Successfully",26,0);
		}
		else if(choice == 2){
			int acc, found=0;
			recv(newsockfd,&acc,sizeof(acc),0);
			for(int i=0;i<count;i++){
				if(books[i].acc_no==acc){
					books[i]=books[count -1];
					count--;
					found=1;
					break;
				}
			}
			if (found){
				send(newsockfd,"Book deleted successfully",26,0);
			}
			else{
				send(newsockfd,"Book Not found",15,0);
			}
		}
		else if(choice==3){
			send(newsockfd,&count,sizeof(count),0);
			send(newsockfd,books,sizeof(struct Book)*count,0);
		}

		else if(choice == 4){//search
			char key[50];
			recv(newsockfd,key ,sizeof(key),0);
			int found=0;
			for(int i=0;i<count;i++){
				if(strcmp(books[i].author,key)==0||strcmp(books[i].title,key)==0){
					send(newsockfd,&books[i],sizeof(struct Book),0);
					found=1;
				}
			}
			if(found==0){
				struct Book empty;
				empty.acc_no=-1;
				send(newsockfd,&empty,sizeof(struct Book),0);
			}
		}
		else if(choice ==5) break;
	}
	/*a
	for (i = 0; ; i+=1)
	{
		
		memset(buff, '\0', sizeof(buff));
		recedbytes=recv(newsockfd,buff,sizeof(buff),0);
		if(recedbytes==-1)
		{
			close(sockfd);
			close(newsockfd);
		}
		
		if (buff[0] == 's' && buff[1] == 't' && buff[2] == 'o' && buff[3] == 'p')
		{
			break;
		}
		printf("%s \n",buff);
		memset(buff, '\0', sizeof(buff));
		scanf("%s",buff);
		buff[strlen(buff)] = '\0';
		int s = strlen(buff) * sizeof(char);
		sentbytes=send(newsockfd,buff,s,0);
		if(sentbytes==-1)
		{
			close(sockfd);
			close(newsockfd);
		}
		
		if (buff[0] == 's' && buff[1] == 't' && buff[2] == 'o' && buff[3] == 'p')
		{
			break;
		}
		
	}*/
	close(sockfd);
	close(newsockfd);
	return 0;
}

