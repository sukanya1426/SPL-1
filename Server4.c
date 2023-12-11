#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#define SIZE 1024


void error(const char *msg) {
    perror(msg);
    exit(1);
}
void write_file(int sockfd) {
    int n;
    FILE *fp;
    char filename[20] = "recv."; // Assuming a maximum extension length of 9 characters
    char extension[10]; // Assuming a maximum extension length of 9 characters

    // Receive file extension
    n = recv(sockfd, extension, sizeof(extension) - 1, 0);
    if (n <= 0) {
        perror("[-]Error in receiving file extension.");
        exit(1);
    }
    extension[n] = '\0'; // Null-terminate the received extension

    // Append received extension to the filename
    strcat(filename, extension);

    unsigned char buffer[SIZE];

    fp = fopen(filename, "wb"); // Open file in binary mode
    if (fp == NULL) {
        perror("[-]Error in opening file.");
        exit(1);
    }

    while (1) {
        n = recv(sockfd, buffer, SIZE, 0);
        if (n <= 0) {
            if (n < 0) {
                perror("[-]Error in receiving file.");
            }
            break;
        }
        size_t written = fwrite(buffer, 1, n, fp);
        if (written < n) {
            perror("[-]Error in writing to file.");
            break;
        }
        bzero(buffer, SIZE);
    }

    fclose(fp);
}


int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[255];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    if (argc < 2) {

        fprintf(stderr, "error, no port provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        
        error("ERROR opening socket");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("binding Failed.");

    listen(sockfd, 5); // Maximum number of queued connections is 5

    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0)
        error("error on Accept");

    
    write_file(newsockfd);
    printf("[+]Data written to the file successfully.\n");



    close(newsockfd);
    close(sockfd);
    

    return 0;
}
