//#include <QCoreApplication>
#include <ncurses.h>
#include <cstring>
//*********************************************************************************//
//START from https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <strings.h> // bzero()
#include <sys/socket.h>// connect()
#include <unistd.h> // read(), write(), close()
#define MAX 8
#define PORT 8888
//#define SA struct sockaddr
//END from tcp-server-client-implementation-in-c/
//*********************************************************************************//

#include <locale.h>//русский язык не работает все равно
//#include <clocale>//русский язык не работает все равно

void funcChat(int sockfd) {
    char buff[MAX];

//================ WINDOWS creation ================//
// initscr(); // инициализация ncurses // уже в main

    int nlines = 16;
    int ncols = 32;
    int y1 = 5;
    int x1 = 1;

    WINDOW* winLeft = newwin(nlines, ncols, y1, x1);// создание окна
    box(winLeft, 0, 0);// рамка winLeft
    refresh();

    int y2 = y1;
    int x2 = 40;

    WINDOW* winRight = newwin(nlines, ncols, y2, x2);
    box(winRight, 0, 0);// рамка winRight
    refresh();
//================ WINDOWS creation ================//

    for (;;) {
        memset(&buff, 0, sizeof(buff));
        mvprintw(3, 0,"Enter the Argument Number: ");
        refresh();
        read(sockfd, buff, sizeof(buff));// читаем из сокета в buff
        int argN = atoi(buff);// порядковый номер аргумента
        refresh();

        memset(&buff, 0, sizeof(buff));
        mvprintw(4, 0,"Enter the Argument: ");
        refresh();
        read(sockfd, buff, sizeof(buff));// читаем из сокета в buff
        refresh();

        int qArg = 6;// количество аргументов в таблице
        int xWin = 1;// координаты строки в окне
        int yWin1 = argN;
        int yWin2 = argN  - qArg/2;

        switch (argN){
        case 1: {
            mvwprintw(winLeft, argN, xWin, "arg %d (int): %8d", argN, atoi(buff));//int
            wrefresh(winLeft);
            break;
        }
        case 2: {
            mvwprintw(winLeft, yWin1, xWin, "arg %d (double): %8lf", argN, atof(buff));//double
            wrefresh(winLeft);
            break;
        }
        case 3: {
            buff[strcspn (buff, "\n")] = 0;// replace '\n' with 0
            mvwprintw(winLeft, yWin1, xWin, "arg %d (char*): %8s", argN, buff);//char*
            wrefresh(winLeft);
            break;
        }
        case 4: {
            mvwprintw(winRight, yWin2, xWin, "arg %d (int): %8d", argN, atoi(buff));//int
            wrefresh(winRight);
            break;
        }
        case 5: {
            mvwprintw(winRight, yWin2, xWin, "arg %d (double): %8lf", argN, atof(buff));//double
            wrefresh(winRight);
            break;
        }
        case 6: {
            buff[strcspn (buff, "\n")] = 0;
            mvwprintw(winRight, yWin2, xWin, "arg %d (char*): %8s", argN, buff);//char*
            wrefresh(winRight);
            break;
        }
        } // end of switch

        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
        }// end of for (;;)
    }// end of funcChat()


int main() {
    setlocale(LC_ALL, "Rus");
    initscr(); // инициализация ncurses

    struct sockaddr_in servaddr/*, cli*/;
    memset(&servaddr, 0, sizeof(servaddr));//зануляю

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // socket create and verification
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        mvprintw(0, 0, "Socket creation failed...\n");
        exit(0);
    }
    else{
        mvprintw(0, 0, "Socket successfully created..\n");
        refresh();
    }

    // connect the client socket to server socket
    if (connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) != 0) {
        mvprintw(1, 0, "Connection with the server failed...\n");
        exit(0);
    }
    else {
        mvprintw(1, 0,"Connected to the server..\n");
        refresh();
    }

    funcChat(sockfd);

    close(sockfd);// close the socket
    endwin();
    return 0 ;
}

