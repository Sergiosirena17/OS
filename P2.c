#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>




#define MAX_BUF 1024

int CONT=0;
int main(int argc, char * argv[]){
    __pid_t pid;
    int señal;

    pid = atoi(argv[1]);
    señal =atoi(argv[2]);

    
    int fd;
    char * myFifo = "/tmp/myFifo";
    char buf[MAX_BUF];

    fd = open(myFifo, O_RDONLY);
    while(1){
        read(fd, buf, MAX_BUF);
        printf("Señal FIFO recibida\n Valor aleatorio -> %s\n\n", buf);
        CONT = CONT + 1;
        kill(pid,señal);
        sleep(2);
        if (CONT >= 10) { 
            exit(0);
        }
    }
    close(fd);

    return 0;
}