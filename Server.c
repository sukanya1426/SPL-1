#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

void error(const char *msg){
     perror(msg);
     exit(1);
}

int main(int argc,char *argv[])
{
    int sockfd,newsockfd,portno;
    socklen_t clilen;
    char buffer[255];
    struct sockaddr_in serv_adder,cli_adder;
    int n;

    if(argc< 2){
        fprintf(stderr," error, not port provided \n");
        exit(1);
    }
    sockfd = socket(AF_INET,SOCK_STREAM,0);

    if(sockfd<0){
        error("ERROR opening socket");
        server = gethosebyname(argv[1]);

    }
    if(server == NULL){
        fprintf(stderr,"error,no such host\n ");
        exit(0);
    }
    bzero((char*)&ser_addr,sizeof(serv_addr));
    portno= atoi(argv[1]);

    serv_adder.sin_family = AF_INET;
    serv_adder.sin_addr.s_addr = INADDR_ANY;
    serv_adder.sin_port = htons(portno);

   if(bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
       error("binding Failed.");
   
   listen(sockfd,s);
   clilen = sizeof(cli_adder);

   newsockfd = accept(sockfd,(struct sockaddr *)&cli_addr,&clilen);

   if(newsockfd<0)
   error("error on Accept");

   while(1)
   {
       bzero(buffer,255);
       n= read(newsockfd,buffer,255);

       if(n<0)
           error("Error on reading . ");

        printf("client : %s\n",buffer);
        bzero(buffer,255);
        fgets(buffer,255,stdin);

        n = write(newsockfd , buffer , strlen(buffer));
        if(n<0)
            error("Error on writing.");
        int i = strncmp("Bye",buffer,3);
        if(i==0)
        break;
    }

    close(newsockfd);
    close(sockfd);

    return 0;




   }



