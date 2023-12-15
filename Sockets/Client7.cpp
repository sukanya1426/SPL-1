#include <stdio.h>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>
#include "Aes.cpp"
using namespace std;
#define SIZE 4000



void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void send_file(FILE *fp, int sockfd, char *filename)
{
    char data[SIZE]; // print korar jonno ekhane unsigned kora hoise
    size_t bytesRead;

    // Get the file extension
    char *extension = strrchr(filename, '.');
    if (extension == NULL)
    {
        perror("[-]Error: No file extension.");
        exit(1);
    }

    cout << "Extension: " << extension << endl;

    // Send the file extension
    size_t extensionLength = strlen(extension + 1);
    cout << "ExtensionLength" << extensionLength << endl; // +1 to skip the dot in the extension
    if (send(sockfd, &extensionLength, sizeof(size_t), 0) == -1)
    {
        perror("[-]Error in sending file extension length.");
        exit(1);
    }

    if (send(sockfd, extension + 1, strlen(extension + 1), 0) == -1)
    {
        perror("[-]Error in sending file extension.");
        exit(1);
    }

    // Send the file content
    while ((bytesRead = fread(data, 1, sizeof(data), fp)) > 0)
    {
        // Encrypt the data using AES encryption
        int txtLen = bytesRead;

        int extendedLen;

        if (txtLen % 16 != 0)
            extendedLen = txtLen + (16 - (txtLen % 16));
        else
            extendedLen = txtLen;

        // Padding the extra bytes
        unsigned char encryptedTxt[extendedLen + 1];

        for (int i = 0; i < extendedLen; i++)
        {
            if (i < txtLen)
                encryptedTxt[i] = data[i];
            else
                encryptedTxt[i] = paddingCharacter;
            if (i + 1 == extendedLen)
                encryptedTxt[i + 1] = '\0';
        }
        for (int i = 0; i < extendedLen; i += 16)
        {
            unsigned char temp[16];
            for (int j = 0; j < 16; j++)
                temp[j] = encryptedTxt[i + j];
           
                encryption(temp); 
            for (int j = 0; j < 16; j++)
                encryptedTxt[i + j] = temp[j];

              
              
        }
         printEncrypt(encryptedTxt,sizeof(encryptedTxt));
        if (send(sockfd, encryptedTxt, extendedLen, 0) == -1)
        {
            perror("[-]Error in sending file content.");
            exit(1);
        }
        bzero(encryptedTxt, extendedLen+1); // age SIZE chilo
    }
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Error opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Error, no such host\n");
        exit(1); // Exit with an error code
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length); //transforming information to
    serv_addr.sin_port = htons(portno);                                                  //host to network

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Connection failed");

    bzero(buffer, 255);
    FILE *fp;
    char filename[256];

    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0; // Remove newline character

    fp = fopen(filename, "rb");
    ifstream inputFile(filename);

    if (!inputFile)
    {
        cout << "File not found or cannot be opened. Exiting..." << endl;
        return 1;
    }

    send_file(fp, sockfd, filename); // +1 to skip the dot in the extension
    printf("[+]File data sent successfully.\n");

    close(sockfd);

    return 0;
}