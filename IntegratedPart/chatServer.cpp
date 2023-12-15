// #include <iostream>
// #include <fstream>
// #include <string>
// #include <unistd.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>

// using namespace std;
//#define PORTNO 9898

 void Chat_Server_error(const char *msg) {
     perror(msg);
     exit(1);
 }

void chatServer(int argc,char **argv)
{
     int port;
     printf("enter portno : ");
     scanf("%d",&port);
     getchar();
    //  if (argc < 2) {
    //     fprintf(stderr, "port number is not provided\n");
    //     fprintf(stderr, "program terminated\n");
    // }

    int sockfd, newsockfd, portno, n;
    char buffer[256];

    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        Chat_Server_error("Error opening the socket\n");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));

    //portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        Chat_Server_error("Binding failed");
    } else {
        printf("Binding successful! Server is listening for connections...\n");
    }

    listen(sockfd, 1000);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0) {
        Chat_Server_error("Error while executing accept.\n");
    }

    char clientName[256];
    bzero(clientName, sizeof(clientName));

    // Read client's name
    n = read(newsockfd, clientName, sizeof(clientName));

    if (n < 0) {
        Chat_Server_error("Error reading from socket");
    }

    // Open a file to store the conversation history
    string historyFileName = "server_" + string(clientName) + "_history.txt";
    ofstream historyFile(historyFileName, ios::app);

    if (!historyFile.is_open()) {
        cerr << "Error opening the history file." << endl;
      //  return 1;
    }

    cout << "Server is ready to chat with " << clientName << ". Type 'Bye' to end the chat.\n";

    while (true) {
        bzero(buffer, sizeof(buffer));
        n = read(newsockfd, buffer, sizeof(buffer));

        if (n < 0) {
            Chat_Server_error("Error reading from socket");
        }

        // Write the client's message to the history file
        historyFile << string(clientName) << ": " << buffer;

        cout << string(clientName) << ": " << buffer;

        if (strncmp(buffer, "Bye", 3) == 0) {
            cout << clientName << " has ended the chat. Goodbye!\n";
            break;
        }

        bzero(buffer, sizeof(buffer));
        cout << "You: ";
        fgets(buffer, sizeof(buffer), stdin);

        // Write the server's response to the history file
        historyFile << "Server: " << buffer;

        n = write(newsockfd, buffer, strlen(buffer));

        if (n < 0) {
            Chat_Server_error("Error writing to socket");
        }

        if (strncmp(buffer, "Bye", 3) == 0) {
            cout << "Ending the chat. Goodbye!\n";
            break;
        }
    }

    // Close the history file
    historyFile.close();

    close(newsockfd);
    close(sockfd);


}

// int main(int argc, char **argv) {
    
//     chatServer( argc, argv);

//     return 0;
// }
