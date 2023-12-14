// #include <iostream>
// #include <fstream>
// #include <stdio.h>
// #include <unistd.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <netdb.h>
//#define PORT_NO 9898


using namespace std;

 void Chat_Client_error(const char *msg) {
     perror(msg);
     exit(1);
 }

void chatClient(int argc,char **argv)
{
    
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    string clientName;
    char address[50];

    printf("enter portno : ");
    scanf("%d",&portno);
    getchar();

    printf("enter the loopback address :");
    scanf("%s",address);
    getchar();

    // if (argc < 3) {
    //     fprintf(stderr, "usage %s hostname port\n", argv[0]);
    //     exit(0);
    // }

   // portno = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        Chat_Client_error("Error opening socket");
    }

    server = gethostbyname(address);
    if (server == NULL) {
        fprintf(stderr, "Error, no such host\n");
        exit(1);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        Chat_Client_error("Connection failed");
    }

    cout << "Socket connection has been established! What is your name?: ";
    getline(cin, clientName);

    n = write(sockfd, clientName.c_str(), clientName.length());

    if (n < 0) {
        Chat_Client_error("Error writing to socket");
    }

    // Open a file to store the conversation history
    string historyFileName = "client_" + clientName + "_history.txt";
    ofstream historyFile(historyFileName, ios::app);

    if (!historyFile.is_open()) {
        cerr << "Error opening the history file." << endl;
        //return 1;
    }

    cout << "You can now start chatting. Type 'Bye' to end the chat.\n";

    while (true) {
        bzero(buffer, sizeof(buffer));
        cout << clientName << ": ";
        fgets(buffer, sizeof(buffer), stdin);

        // Write the message to the history file
        historyFile << clientName << ": " << buffer;

        n = write(sockfd, buffer, strlen(buffer));

        if (n < 0) {
            Chat_Client_error("Error writing to socket");
        }

        if (strncmp(buffer, "Bye", 3) == 0) {
            cout << "Ending the chat. Goodbye!\n";
            break;
        }

        bzero(buffer, sizeof(buffer));
        n = read(sockfd, buffer, sizeof(buffer));

        if (n < 0) {
            Chat_Client_error("Error reading from socket");
        }

        // Write the server's response to the history file
        historyFile << "Server: " << buffer;

        cout << "Server: " << buffer;
        
        if (strncmp(buffer, "Bye", 3) == 0) {
            cout << "Server has ended the chat. Goodbye!\n";
            break;
        }
    }

    // Close the history file
    historyFile.close();

    close(sockfd);


}

// int main(int argc, char **argv) {
    
//     chatClient(argc,argv);

//     return 0;
// }
