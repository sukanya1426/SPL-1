#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<ctype.h>

void error(const char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[1024];

    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Error opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "Error, no such host\n");
        exit(1);  // Exit with an error code
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length); //transforming information to 
    serv_addr.sin_port = htons(portno);  //host to network

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Connection failed");

    bzero(buffer,255);
    FILE *fp;
    int words=0;
    long ch;
    fp=fopen("dwsample-bmp-640.bmp","rb");
    if(fp == NULL)
    {
        perror("Error opening file");
        exit(1);
    }
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    rewind(fp);

    write(sockfd,&fileSize,sizeof(long));

    while(ch!= fileSize){
        size_t bytesRead = fread(buffer,1,sizeof(buffer),fp);
        if(bytesRead <=0){
            perror("Error reading file");
            exit(1);
        }
        write(sockfd,buffer,bytesRead);
        ch += bytesRead;
    }
    printf("the  BMP file has been successfully sent.thank you.");
    close(sockfd);

    return 0;
}
