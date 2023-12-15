// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>

#define SIZE2 4000

void Serror(const char *msg)
{
    perror(msg);
    exit(1);
}

// void send_file(FILE *fp, int sockfd, char *filename)
// {
//     char data[SIZE];
//     size_t bytesRead;

//     // Get the file extension
//     char *extension = strrchr(filename, '.');
//     if (extension == NULL)
//     {
//         perror("[-]Error: No file extension.");
//         exit(1);
//     }

//     // Send the file extension
//     size_t extensionLength = strlen(extension + 1);
//     if (send(sockfd, &extensionLength, sizeof(size_t), 0) == -1)
//     {
//         perror("[-]Error in sending file extension length.");
//         exit(1);
//     }

//     if (send(sockfd, extension + 1, strlen(extension + 1), 0) == -1)
//     {
//         perror("[-]Error in sending file extension.");
//         exit(1);
//     }

//     // Send the file content
//     while ((bytesRead = fread(data, 1, sizeof(data), fp)) > 0)
//     {
//         if (send(sockfd, data, bytesRead, 0) == -1)
//         {
//             perror("[-]Error in sending file content.");
//             exit(1);
//         }
//         bzero(data, SIZE);
//     }
// }

void send_file2(FILE *fp, int sockfd, char *filename)
{
    char data[SIZE2];
    size_t bytesRead;

    // Send the file content
    while ((bytesRead = fread(data, 1, sizeof(data), fp)) > 0)
    {
        if (send(sockfd, data, bytesRead, 0) == -1)
        {
            perror("[-]Error in sending file content.");
            exit(1);
        }
        bzero(data, SIZE2);
    }
}
void HistoryServer(int argc, char *argv[])
{
  int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    printf("Enter the portno: ");
    scanf("%d",&portno);

    // if (argc < 2)
    // {
    //     fprintf(stderr, "error, no port provided\n");
    //     exit(1);
    // }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        Serror("ERROR opening socket");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    //portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        Serror("binding Failed.");

    listen(sockfd, 5); // Maximum number of queued connections is 5

    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0)
        Serror("error on Accept");

    // Receive the name from the client
    char name[256];
    if (recv(newsockfd, name, sizeof(name), 0) == -1)
    {
        perror("[-]Error in receiving name from client.");
        exit(1);
    }

    // Check if the history file exists
    char history_filename[400];
    sprintf(history_filename, "client_%s_history.txt", name);
    FILE *fp = fopen(history_filename, "rb");
    if (fp == NULL)
    {
        perror("[-]Error: History file does not exist.");
        exit(1);
    }

    // Send the file content to the client
    send_file2(fp, newsockfd, history_filename);
    printf("[+]File data sent successfully.\n");

    fclose(fp);
    close(newsockfd);
    close(sockfd);


}


// int main(int argc, char *argv[])
// {
//     HistoryServer(argc,argv);

//     return 0;
// }
