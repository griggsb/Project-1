//client for ftp
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define MAXLINE 1024
int main(void)
{
    int conQ = 0;
    char buffer[1024];
    int sockfd=0;
    int len, n, num;
    struct sockaddr_in serv_addr, cliaddr;
    while(1){
   	 printf("cmd:");
	 char resp[100];
  	 fgets(resp,100,stdin);
	 char * resps;
	 char * holder;
	 strcpy(holder,resp);
	 resps = strtok(resp," "); 
	 if(strcmp(resps,"QUIT\n")== 0){
		 printf("quit");
		 close(sockfd);
	 	 break;
	 }
	 if(strcmp(resps,"CONNECT")== 0){
			printf("\n");
		char* ipa;
		char* portn;
    		/* Create a socket first */
    		if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
  		{
        		printf("\n Error : Could not create socket \n");
        		return 1;
    		}

    		/* Initialize sockaddr_in data structure */
    		serv_addr.sin_family = AF_INET;
		resps = strtok(NULL, " ");
		ipa = malloc(strlen(resps) + 1);
		strcpy(ipa,resps);
		resps = strtok(NULL, "\n");
		portn = malloc(strlen(resps) + 1);
		strcpy(portn,resps);
    		serv_addr.sin_port = htons(atoi(portn)); // port
		serv_addr.sin_addr.s_addr = inet_addr(ipa);
   		/* Attempt a connection */
		if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
   		{
       		    printf("\n Error : Connect Failed \n");
   		    return 1;
   		}
		printf("Connected\n");
		conQ=1;
	 }
	 
	 if(strcmp(resps,"LIST\n")== 0){
		if(conQ == 1){
			char* listcmd;
			listcmd = malloc(strlen(resps) + 1);
			strcpy(listcmd,resps);
			if((send(sockfd,listcmd,strlen(listcmd),0))==-1){
				printf("send failure");
				close(sockfd);
				exit(1);
			}
			else {
				printf("message sent\n");
			}
			n = recv(sockfd, (char *)buffer,MAXLINE,0); 
    			buffer[n] = '\0'; 
    			printf("%s\n",buffer); 
			if((send(sockfd,listcmd,strlen(listcmd),0))==-1){
	       		printf("\n");
	 		}		
		}
	 }

	 if(strcmp(resps,"RETRIEVE")== 0){

		 if((send(sockfd,holder,strlen(holder),0))!=-1){
		 }

		 printf("retrieving");
	 	 break;
	 }
	
	 if(strcmp(resps,"STORE")== 0){
		 if((send(sockfd,holder,strlen(holder),0))!=-1){
			resps = strtok(holder," ");
			resps = strtok(NULL,"\n");
			char* filen = malloc(strlen(resps)+1);
			strcpy(filen,resps);
			FILE *f;
			f=fopen(filen,"r");

			while(fscanf(f,"%1023[^\n]",buffer)==1){
				char * hold1 = malloc(strlen(buffer) +1);
				strcpy(hold1,buffer);
				write (sockfd,hold1,1024);
			}
			printf("file sent");
		 }
	 	 break;
	 }
    }

    return 0;
}
