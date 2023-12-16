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
// #include "Aess.cpp"
// using namespace std;
 #include<SDL2/SDL.h>
 #include<SDL2/SDL_ttf.h>
 #include <thread>
 #include <chrono>

//#include"../../sdl/src/ServerBox.cpp"
#define SIZE 4000

 void delay(int milliseconds) {
     std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
 }

 void box() {
     SDL_Window* window = SDL_CreateWindow("File is sending", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 480, SDL_WINDOW_SHOWN);
     SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
     SDL_Init(SDL_INIT_VIDEO);
     TTF_Init();

     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
     SDL_RenderClear(renderer);

     // Draw Server box
     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
     SDL_Rect serverBoxRect = {50, 50, 200, 100};
     SDL_RenderDrawRect(renderer, &serverBoxRect);
     SDL_RenderFillRect(renderer, &serverBoxRect);

     TTF_Font* font = TTF_OpenFont("arial.ttf", 25);
     SDL_Color color = {0, 0, 0};
     SDL_Surface* serverSurface = TTF_RenderText_Solid(font, "Server", color);
     SDL_Texture* serverTexture = SDL_CreateTextureFromSurface(renderer, serverSurface);

     int serverTexW = 0;
     int serverTexH = 0;
     SDL_QueryTexture(serverTexture, NULL, NULL, &serverTexW, &serverTexH);

     SDL_Rect serverDstrect = {(serverBoxRect.w - serverTexW) / 2 + serverBoxRect.x, (serverBoxRect.h - serverTexH) / 2 + serverBoxRect.y, serverTexW, serverTexH};
     SDL_RenderCopy(renderer, serverTexture, NULL, &serverDstrect);

     // Draw Loading dots
     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
     for (int i = 0; i < 6; ++i) {
         SDL_Rect dotRect = {serverBoxRect.x + serverBoxRect.w + 30 + i * 20, serverBoxRect.y + serverBoxRect.h / 2 - 5, 10, 10};
         SDL_RenderFillRect(renderer, &dotRect);
         SDL_RenderPresent(renderer);
         delay(1000);  // Delay for 1 second between dots
     }

     // Draw Client box
     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
     SDL_Rect clientBoxRect = {serverBoxRect.x + serverBoxRect.w + 200, 50, 200, 100};  // Added distance between boxes
     SDL_RenderDrawRect(renderer, &clientBoxRect);
     SDL_RenderFillRect(renderer, &clientBoxRect);

     SDL_Surface* clientSurface = TTF_RenderText_Solid(font, "Client", color);
     SDL_Texture* clientTexture = SDL_CreateTextureFromSurface(renderer, clientSurface);

     int clientTexW = 0;
     int clientTexH = 0;
     SDL_QueryTexture(clientTexture, NULL, NULL, &clientTexW, &clientTexH);

     SDL_Rect clientDstrect = {(clientBoxRect.w - clientTexW) / 2 + clientBoxRect.x, (clientBoxRect.h - clientTexH) / 2 + clientBoxRect.y, clientTexW, clientTexH};
     SDL_RenderCopy(renderer, clientTexture, NULL, &clientDstrect);

     SDL_RenderPresent(renderer);

    // delay(3000);  // Delay for 3 seconds
         // Event loop
     bool quit = false;
     SDL_Event e;

     // Event loop
     while (!quit) {
         // Handle events
         while (SDL_PollEvent(&e) != 0) {
             if (e.type == SDL_QUIT) {
                 quit = true; // User closed the window
             }
         }
     }

     SDL_DestroyRenderer(renderer);
     SDL_DestroyWindow(window);
     SDL_DestroyTexture(serverTexture);
     SDL_FreeSurface(serverSurface);
     SDL_DestroyTexture(clientTexture);
     SDL_FreeSurface(clientSurface);
     TTF_CloseFont(font);
     TTF_Quit();
     SDL_Quit();

    
 }


 void Client_error(const char *msg)
 {
     perror(msg);
     exit(0);
 }

void send_file(FILE *fp, int sockfd, char *filename)
{
    char data[SIZE]; 
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
        //printEncrypt(encryptedTxt,sizeof(encryptedTxt));
         printf("\n \n");
        if (send(sockfd, encryptedTxt, extendedLen, 0) == -1)
        {
            perror("[-]Error in sending file content.");
            exit(1);
        }
        bzero(encryptedTxt, extendedLen+1); // age SIZE chilo
    }
}

void ClientSoc(int argc,char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    char address[50];
    printf("enter the portno : ");
    scanf("%d",&portno);
    getchar();
    
    printf("enter the loopback address : ");
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
        Client_error("Error opening socket");

    server = gethostbyname(address);
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
        Client_error("Connection failed");

    bzero(buffer, 255);
    FILE *fp;
    char filename[256];

    printf("Enter the file name: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0; // Remove newline character

    fp = fopen(filename, "rb");
    ifstream inputFile(filename);

    if (!inputFile)
    {
        cout << "File not found or cannot be opened. Exiting..." << endl;
        //return 1;
        
    }
     std::thread boxThread(box);   // this is added new

    send_file(fp, sockfd, filename); // +1 to skip the dot in the extension
    printf("[+]File data sent successfully.\n");
    
    boxThread.join();  // this is added new

    close(sockfd);

}

// int main(int argc, char *argv[])
// {
//     ClientSoc(argc,argv);


//     return 0;
// }
