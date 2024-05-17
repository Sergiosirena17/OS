#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h> // Necesario para usleep

#define SHMSZ 27

int main() {
    int shmid;
    key_t key = 5678;
    char *shm, *s;

    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    char lastvalue[100] = "";
    int newvalue = 100;
    bool first = true;
    char buff[100];

    while (1) {
        strncpy(buff, shm, 20);
        buff[20] = '\0';

        if (first) {
            printf("PRIMERA VUELTA\n");
            char str[20];
            sprintf(str, "%d", newvalue);
            strncpy(lastvalue, str, 20);
            printf("Nuevo valor: %d\n", newvalue);
            strncpy(shm, lastvalue, 20);
            first = false;
        } else {
            if (strcmp(buff, lastvalue) != 0) {
                newvalue++;
                printf("Nuevo valor: %d\n", newvalue);

                char str[20];
                sprintf(str, "%d", newvalue);

                strncpy(lastvalue, str, 20);
                strncpy(shm, lastvalue, 20);
            }
        }

        usleep(1000000); 
    }

    exit(0);
}
