#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHMSZ 27

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <mensaje>\n", argv[0]);
        exit(1);
    }

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

    strncpy(shm, argv[1], 20);
    shm[20] = '\0'; // Asegurar la terminación de cadena

    printf("Mensaje escrito en la memoria compartida: %s\n", argv[1]);

    exit(0);
}
