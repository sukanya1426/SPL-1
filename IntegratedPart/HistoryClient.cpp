// #include <stdio.h>
// #include <bits/stdc++.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <netdb.h>
// #include <ctype.h>

using namespace std;

#define SIZE1 4000

void Cerror(const char *msg)
{
    perror(msg);
    exit(0);
}

void print_received_data(int sockfd)
{
    int n;
    unsigned char buffer[SIZE1];

    while (1)
    {
        n = recv(sockfd, buffer, SIZE1, 0);
        if (n <= 0)
        {
            if (n < 0)
            {
                perror("[-]Error in receiving data.");
            }
            break;
        }
        write(1, buffer, n); // Print to the terminal (file descriptor 1)
        bzero(buffer, SIZE1);
    }
}
void HistoryClient(int argc,char *argv[])
{
int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    char address[50];
    printf("Enter the portno : ");
    scanf("%d",&portno);
    getchar();
    printf("Enter the loopback address : ");
    scanf("%s",address);
    getchar();

    // if (argc < 3)
    // {
    //     fprintf(stderr, "usage %s hostname port\n", argv[0]);
    //     exit(0);
    // }

   // portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        Cerror("Error opening socket");

    server = gethostbyname(address);
    if (server == NULL)
    {
        fprintf(stderr, "Error, no such host\n");
        exit(1); // Exit with an error code
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        Cerror("Connection failed");

    // Get the name from the user
    printf("Enter a name of the chat you want to read : ");
    char name[256];
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0; // Remove newline character

    // Send the name to the server
    if (send(sockfd, name, strlen(name), 0) == -1)
    {
        perror("[-]Error in sending name to server.");
        exit(1);
    }

    // Receive the file content from the server and print it to the terminal
    print_received_data(sockfd);

    close(sockfd);

}

// int main(int argc, char *argv[])
// {
//     HistoryClient(argc,argv);
//     return 0;
// }
