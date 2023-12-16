#include <stdio.h>
// #include<SDL2/SDL.h>
// #include<SDL2/SDL_ttf.h>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>
#include "Aess.cpp"
#include "Server5.cpp"
#include "Client5.cpp"
#include "chatClient.cpp"
#include "chatServer.cpp"
#include "HistoryClient.cpp"
#include "HistoryServer.cpp"
using namespace std;
#define SIZE 4000
// #define Client_side_SIZE 4000
// #define Server_side_SIZE 1024


void exitProgram() {
    printf("Exiting the program...\n");
}   

int main(int argc, char **argv)
{

    int choice ;
    printf("-----------  MENU  -----------\n\n");

    printf("1.Chat with server  .\n\n2.Send a file to server .\n\n3.Read chat history .\n\n");
    

    while(1)
    {
    printf("Enter a number (1-3) to perform a task and enter 0 to exit");
    scanf("%d",&choice);

    switch (choice)
    {
    case 1 :
    {
        printf("1.To operate as a server\n2.To operate as a client\n");
        int x;
        scanf("%d",&x);
        getchar();
        if(x==1){
             chatServer(argc,argv);
       }
        else
        chatClient(argc,argv);
        break;
    }
    case 2 :
    {
        printf("1.To operate as a server\n2.To operate as a client\n");
        int x;
        scanf("%d",&x);
        getchar();
        if(x==1){
             ServerSoc(argc,argv);
       }
        else
           ClientSoc(argc,argv);
       break;
    }
     case 3 :
   {
        printf("1.To operate as a server\n2.To operate as a client\n");
        int x;
        scanf("%d",&x);
        getchar();
        if(x==1){
             HistoryServer(argc,argv);
       }
        else
            HistoryClient(argc,argv);
       break;



   }
     case 0:
        exitProgram();
        return 0; 

    default:
    printf("Invalid choice. Please enter a number between 1 and 3, or 0 to exit.\n");

   }
    
  }
  
    return 0;
}
