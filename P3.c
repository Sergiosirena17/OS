#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>





int CONT=0;
int signalON=0;

void sigchld_handler(int sig){
    signalON = 1;
}


int main (int argc, char * argv[]) {
   __pid_t pid;
   int señal;

   pid = atoi(argv[1]);
   señal =atoi(argv[2]);

   signal(SIGINT, sigchld_handler);

   while(1){
      if(signalON){
         signalON=0;
         kill(pid,señal);
         CONT=CONT+1;
         printf("Señal %d de p2 recibida\n", CONT);
      }
      if (CONT>=10){
            printf("TERMINAR PROGRAMA\n");
            exit(0);
         }
   }

   
   return(0);
}