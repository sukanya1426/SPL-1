 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <unistd.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include"Aess.cpp"
 #define SERVER_SIDE_SIZE 1024


 void Server_error(const char *msg) {
     perror(msg);
     exit(1);
 }
// void write_file(int sockfd) {
//     int n;
//     FILE *fp;
//     char filename[100] = "recv."; // Assuming a maximum extension length of 9 characters
//   //  char extension[10]; // Assuming a maximum extension length of 9 characters
    
//     // Receive file extension length
//     size_t extensionLength;
//     n = recv(sockfd, &extensionLength, sizeof(size_t), 0);
//     if (n <= 0) {
//     perror("[-]Error in receiving file extension length.");
//     exit(1);
//      }

//      cout<<"Extension length: "<<extensionLength<<endl;

//     // Receive file extension

//     char *extension = (char *)malloc(extensionLength + 1);
//     if (extension == NULL) {
//         perror("[-]Error in dynamic memory allocation.");
//         exit(1);
//     }

//     n = recv(sockfd, extension, extensionLength, 0);
//     if (n <= 0) {
//         perror("[-]Error in receiving file extension.");
//         free(extension);
//         exit(1);
//     }
//     extension[extensionLength] = '\0'; // Null-terminate the received extension

//     // Append received extension to the filename
//     strcat(filename, extension);
//     free(extension);

//     unsigned char buffer[SERVER_SIDE_SIZE];                       // to resolve error
    
//     fp = fopen(filename, "wb"); // Open file in binary mode
//     if (fp == NULL) {
//         perror("[-]Error in opening file.");
//         exit(1);
//     }

//     while (1) {
//         n = recv(sockfd, buffer, SERVER_SIDE_SIZE , 0);// sudhu buffer chilo ekhane
//         if (n <= 0) {
//             if (n < 0) {
//                 perror("[-]Error in receiving file.");
//             }
//             break;
//         }
//        for (size_t i = 0; i < n; i += 16) {
//             unsigned char temp[16];
//             for (size_t j = 0; j < 16; j++)
//                 temp[j] = buffer[i + j];
//             decryption(temp);

//            for (size_t j = 0; j < 16; j++) {
//                 if (temp[j] != '$') {
//                     buffer[i + j] = temp[j];
//                 } else {
//                     buffer[i + j] = ' '; // Replace '$' with a space
//                 }
//             }

//         }
//         printDecrypt(buffer,sizeof(buffer));
//         printf("\n \n");


//         size_t written = fwrite(buffer, 1, n, fp);
//         if (written < n) {
//             perror("[-]Error in writing to file.");
//             break;
//         }
//         bzero(buffer, SERVER_SIDE_SIZE);
//     }
//     //free(extension);

//   //  extension = nullptr;
//     fclose(fp);
// }
 void write_file(int sockfd) {
    int n;
    FILE *fp;
    char filename[100] = "recv."; // Assuming a maximum extension length of 9 characters
    
    // Receive file extension length
    size_t extensionLength;
    n = recv(sockfd, &extensionLength, sizeof(size_t), 0);
    if (n <= 0) {
        perror("[-]Error in receiving file extension length.");
        exit(1);
    }

    cout << "Extension length: " << extensionLength << endl;

    // Receive file extension
    char *extension = (char *)malloc(extensionLength + 1);
    if (extension == NULL) {
        perror("[-]Error in dynamic memory allocation.");
        exit(1);
    }

    n = recv(sockfd, extension, extensionLength, 0);
    if (n <= 0) {
        perror("[-]Error in receiving file extension.");
        free(extension);
        exit(1);
    }
    extension[extensionLength] = '\0'; // Null-terminate the received extension

    // Append received extension to the filename
    strcat(filename, extension);
    free(extension);

    unsigned char buffer[SERVER_SIDE_SIZE];
    
    fp = fopen(filename, "wb"); // Open file in binary mode
    if (fp == NULL) {
        perror("[-]Error in opening file.");
        exit(1);
    }

    while (1) {
        n = recv(sockfd, buffer, SERVER_SIDE_SIZE, 0);
        if (n <= 0) {
            if (n < 0) {
                perror("[-]Error in receiving file.");
            }
            break;
        }

        for (size_t i = 0; i < n; i += 16) {
            unsigned char temp[16];
            for (size_t j = 0; j < 16; j++)
                temp[j] = buffer[i + j];

            decryption(temp);

           
            for (size_t j = 0; j < 16; j++) {
                if (temp[j] != '$') {
                    buffer[i + j] = temp[j];
                } else {
                    buffer[i + j] = ' '; // Replace '$' with a space
                }
            }
        }
        printDecrypt(buffer,sizeof(buffer));
        size_t written = fwrite(buffer, 1, n, fp);
        if (written < n) {
            perror("[-]Error in writing to file.");
            break;
        }
        bzero(buffer, SERVER_SIDE_SIZE);
    }

    fclose(fp);
}




void ServerSoc(int argc,char **argv)
{
    int sockfd, newsockfd, portno;
    socklen_t clilen ;
    char buffer[255];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    if (argc < 2) {

        fprintf(stderr, "error, no port provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        
         Server_error("ERROR opening socket");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        Server_error("binding Failed.");

    listen(sockfd, 5); // Maximum number of queued connections is 5

    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0)
        Server_error("error on Accept");

    
    write_file(newsockfd);
    printf("[+]Data written to the file successfully.\n");



    close(newsockfd);
    close(sockfd);
    
}


 int main(int argc, char **argv) {
     ServerSoc( argc,argv);
    

     return 0;
 }
