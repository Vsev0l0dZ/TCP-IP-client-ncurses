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
#define MIN_INT -1000
#define MAX_INT 1000
#define MIN_DOUBLE -1000.0
#define MAX_DOUBLE 1000.0

//#define SA struct sockaddr
//END from tcp-server-client-implementation-in-c/
//*********************************************************************************//

#include <locale.h>
//#include <clocale>

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
    int x2 = 36;

    WINDOW* winRight = newwin(nlines, ncols, y2, x2);
    box(winRight, 0, 0);// рамка winRight
    refresh();
//================ WINDOWS creation ================//

    for (;;) {
        memset(&buff, 0, sizeof(buff));
        mvprintw(4, 0,"Enter the Argument No. (from 1 to 6): ");
        refresh();
        recv(sockfd, buff, sizeof(buff), 0);// читаем из сокета в buff
        int argN = atoi(buff);// порядковый номер аргумента
        refresh();
        memset(&buff, 0, sizeof(buff));


        while((argN < 1) || (argN > 6)){
            memset(&buff, 0, sizeof(buff));
            refresh();
            mvprintw(4, 0,"Enter the correct Argument No.!:        ");// добавил пробелов to clear prev. words
            refresh();
            recv(sockfd, buff, sizeof(buff), 0);
            argN = atoi(buff);// порядковый номер аргумента
            refresh();
        }

        mvprintw(4, 0,"Enter the Value of the Argument No.%d:             ", argN);// добавил пробелов to clear prev. words
        refresh();
        recv(sockfd, buff, sizeof(buff), 0);// читаем из сокета в buff


//        while(strlen(buff) > MAX){
//            memset(&buff, 0, sizeof(buff));
//            mvprintw(4, 0,"Enter the Correct Argument!");
//        }

//        while((atoi(buff) < MIN_INT) || (atoi(buff) > MAX_INT)){
//            memset(&buff, 0, sizeof(buff));
//            mvprintw(4, 0,"Enter the Correct Argument!");
//        }

//        while((atof(buff) < MIN_DOUBLE) || (atof(buff) > MAX_DOUBLE)){
//            memset(&buff, 0, sizeof(buff));
//            mvprintw(4, 0,"Enter the Correct Argument!");
//        }


        int qArg = 6;// количество аргументов в таблице
        int xWin = 1;// координаты строки в окне
        int yWin1 = argN;
        int yWin2 = argN  - qArg/2;

        switch (argN){
        case 1: {
            while((atoi(buff) < MIN_INT) || (atoi(buff) > MAX_INT)){
                memset(&buff, 0, sizeof(buff));
                mvprintw(4, 0,"Enter the Correct Argument!");// покрасить в красный жЫрный!
            }
            mvwprintw(winLeft, argN, xWin, "arg %d (int):    %-8.d", argN, atoi(buff));//int
            wrefresh(winLeft);
            break;
        }
        case 2: {
            while((atof(buff) < MIN_DOUBLE) || (atof(buff) > MAX_DOUBLE)){
                memset(&buff, 0, sizeof(buff));
                mvprintw(4, 0,"Enter the Correct Argument!");
            }
            mvwprintw(winLeft, yWin1, xWin, "arg %d (double): %-8lf", argN, atof(buff));//double
            wrefresh(winLeft);
            break;
        }
        case 3: {
            while(strlen(buff) > MAX){
                memset(&buff, 0, sizeof(buff));
                mvprintw(4, 0,"Enter the Correct Argument!");
            }
            buff[strcspn (buff, "\n")] = 0;// replace '\n' with 0
            mvwprintw(winLeft, yWin1, xWin, "arg %d (char*):  %-8s", argN, buff);//char*
            wrefresh(winLeft);
            break;
        }
        case 4: {
            while((atof(buff) < MIN_DOUBLE) || (atof(buff) > MAX_DOUBLE)){
                memset(&buff, 0, sizeof(buff));
                mvprintw(4, 0,"Enter the Correct Argument!");
            }
            while((atoi(buff) < MIN_INT) || (atoi(buff) > MAX_INT)){
                memset(&buff, 0, sizeof(buff));
                mvprintw(4, 0,"Enter the Correct Argument!");// покрасить в красный жЫрный!
            }

            mvwprintw(winRight, yWin2, xWin, "arg %d (int):    %-8d", argN, atoi(buff));//int
            wrefresh(winRight);
            break;
        }
        case 5: {
            while(strlen(buff) > MAX){
                memset(&buff, 0, sizeof(buff));
                mvprintw(4, 0,"Enter the Correct Argument!");
            }
            mvwprintw(winRight, yWin2, xWin, "arg %d (double): %-8lf", argN, atof(buff));//double
            wrefresh(winRight);
            break;
        }
        case 6: {
            while(strlen(buff) > MAX){
                memset(&buff, 0, sizeof(buff));
                mvprintw(4, 0,"Enter the Correct Argument!");// покрасить в красный жЫрный!
            }
            buff[strcspn (buff, "\n")] = 0;
            mvwprintw(winRight, yWin2, xWin, "arg %d (char*):  %-8s", argN, buff);//char*
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

