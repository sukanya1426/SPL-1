#include <stdio.h>
#include<bits/stdc++.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<ctype.h>
#include"Aess.cpp"
using namespace std;
#define SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(0);
}

void send_file(FILE *fp, int sockfd,char*filename) {
    unsigned char data[SIZE];
    size_t bytesRead;

    // Get the file extension
    char *extension = strrchr(filename, '.');
    if (extension == NULL) {
        perror("[-]Error: No file extension.");
        exit(1);
    }

    cout<<"Extension: "<<extension<<endl;

    // Send the file extension

    size_t extensionLength = strlen(extension + 1); 
    cout<<"ExtensionLength"<<extensionLength<<endl; // +1 to skip the dot in the extension
   if (send(sockfd, &extensionLength, sizeof(size_t), 0) == -1) {
    perror("[-]Error in sending file extension length.");
    exit(1);
    }

    if (send(sockfd, extension + 1, strlen(extension + 1), 0) == -1) {
        perror("[-]Error in sending file extension.");
        exit(1);
    }

    // Send the file content
    while ((bytesRead = fread(data, 1, sizeof(data), fp)) > 0) {
        //Encrypt the data using AES encryption
        for(size_t i=0;i< bytesRead;i+=16){
            unsigned char temp[16];
            for(size_t j=0;j<16;j++)
               temp[j] = data[i+j];
               encryption(temp);
        //    printEncrypt(temp,sizeof(data));
               for(size_t j =0 ; j<16;j++)
                  data[i+j]= temp[j];
        
        }
       // printEncrypt(data,sizeof(data));

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

    fp = fopen(filename, "rb");
      ifstream inputFile(filename);


      if (!inputFile) {
          cout << "File not found or cannot be opened. Exiting..." << endl;
          return 1;
     }

   
    send_file(fp, sockfd, filename); // +1 to skip the dot in the extension
    printf("[+]File data sent successfully.\n");






    close(sockfd);

    return 0;
}
