#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    unsigned char buffer[4000];  // buffer of sufficient size for BMP files
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

    listen(sockfd, 5);  // Maximum number of queued connections is 5

    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0)
        error("error on Accept");

    FILE *fp;
    fp = fopen("file_received.bmp", "wb");  //binary write 

    while (1) {
        n = read(newsockfd, buffer, sizeof(buffer));
        if (n <= 0)
            break;  // No more data to read
        fwrite(buffer, 1, n, fp);
    }

    printf("The BMP file has been received successfully and saved as file_received.bmp.\n");

    fclose(fp);
    close(newsockfd);
    close(sockfd);

    return 0;
}
