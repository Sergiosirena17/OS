#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

char* randomNum() {
    srand(time(NULL));
    int valor = rand() % 1000;
    char* char_valor = (char*)malloc(4 * sizeof(char));
    sprintf(char_valor, "%d", valor);
    return char_valor;
}

void sigchld_handler(int sig);
int CONT=0;
int signalON;
int main()
{
    signalON = 0;
    
    signal(SIGINT, sigchld_handler);
    
    int fd;
    char * myFifo = "/tmp/myFifo";
    mkfifo(myFifo, 0666);
    
    fd = open(myFifo, O_WRONLY);
    while(1){
        if (signalON){
            signalON=0;
            char* a=randomNum();
            write(fd, a, sizeof(a));
            CONT = CONT + 1;
        }
                    if (CONT > 10) { 
                exit(0);
            }
    }
    close(fd);

    unlink(myFifo);

    return 0;
}


void sigchld_handler(int sig){
    signalON = 1;
}