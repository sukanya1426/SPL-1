#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<ctype.h>
#define SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(0);
}

void send_file(FILE *fp, int sockfd,char*filename) {
    char data[SIZE];
    size_t bytesRead;

    // Get the file extension
    char *extension = strrchr(filename, '.');
    if (extension == NULL) {
        perror("[-]Error: No file extension.");
        exit(1);
    }

    // Send the file extension
    if (send(sockfd, extension + 1, strlen(extension + 1), 0) == -1) {
        perror("[-]Error in sending file extension.");
        exit(1);
    }

    // Send the file content
    while ((bytesRead = fread(data, 1, sizeof(data), fp)) > 0) {
        if (send(sockfd, data, bytesRead, 0) == -1) {
            perror("[-]Error in sending file content.");
            exit(1);
        }
        bzero(data, SIZE);
    }
}

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

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
    char filename[256];

    printf("Enter the file name: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;  // Remove newline character

    // Extract file extension
    char *fileExtension = strchr(filename, '.');
    if (fileExtension == NULL) {
    perror("[-]Error: File extension not found.");
    exit(1);
    }
    fp = fopen(filename, "rb");
    if (fp == NULL) {
    perror("[-]Error in reading file.");
    exit(1);
    }

    send_file(fp, sockfd, filename); // +1 to skip the dot in the extension
    printf("[+]File data sent successfully.\n");






    close(sockfd);

    return 0;
}
