//server
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

int main(void)
{
    socklen_t size;
    struct sockaddr_in server;
    struct sockaddr_in dest;
    int newsockfd;
    int listenfd = 0;
    int connfd = 0;
    struct sockaddr_in serv_addr,cliadder;
    char sendBuff[1024];
    int numrv, num, len;
    char buffer[10241];
    char *buff;
    socklen_t addrlen;


    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    printf("Socket retrieve success\n");

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(8080);


//    if(listen(listenfd, 10) == -1)
//    {
//       	printf("Failed to listen\n");
//       	return -1;
//    }

    while(1)
    {


    	bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
        listen(listenfd, 5);
	addrlen = sizeof(serv_addr);
	puts("count me");
	newsockfd = accept(listenfd, (struct sockaddr*) &serv_addr, &addrlen);
	puts("beter be once\n");
	pid_t pID = fork();
	if(pID<0){
		perror("failed to fork");
		exit(1);
	}
	if(pID==0){
	puts("child created");
	close(listenfd);
	int b;
        memset(&sendBuff, '0', sizeof(sendBuff));
	b = recv(newsockfd,sendBuff,sizeof(sendBuff), 0);
	if (b<0){
		printf("can't recieve from client");
	}



        sendBuff[b] = '\0';
        printf("Message received: %s\n", sendBuff);
	if(strcmp(sendBuff,"LIST")){
		DIR *d;
		struct dirent *dir;
		d = opendir(".");
		if(d){
			printf("\n");
			char* hold2 = malloc(2048);
			while((dir = readdir(d))!=NULL){
				char* hold = malloc(strlen(dir->d_name)+1);
				strcpy(hold, dir->d_name);
				strcat(hold, "\n");
				strcat(hold2,hold);
				free(hold);
			}
			send(newsockfd,hold2,strlen(hold2),0);
				//	MSG_CONFIRM, (const struct sockaddr *) &dest,len);
			free(hold2);
			printf("list sent to client");
			closedir(d);
		}
	}
	char * resps;
	resps = strtok(sendBuff, " ");
	if(strcmp(resps,"RETRIEVE")==0){
		resps = strtok(NULL," ");
		char* filen = malloc(strlen(resps)+1);
		strcpy(filen, resps);

		
	}
	if(strcmp(resps,"STORE")==0){
		puts(resps);
		resps = strtok(NULL,"\n");
		puts(resps);
		char* filen = malloc(strlen(resps)+1);
		strcpy(filen, resps);
		FILE *fp;
		char*buffer2 [1024];
		read(newsockfd,buffer2,1024);
		fp = fopen(filen,"w");
		fprintf(fp,"%s",buffer2);
		printf("File recieved\n");
	}
	exit(1);	
  	}
    }
    return 0;
}
