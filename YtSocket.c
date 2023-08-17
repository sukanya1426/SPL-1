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
    int sockfd,newsockfd,partno;
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
    serv_adder.sin_family = AF_INET;
    bcopy((char*)server->h_addr,
          (char*)&serv_adder,sin_addr,s_adder,
          server->h_length);
    serv_adder.
}