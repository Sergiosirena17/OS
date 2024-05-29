#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <errno.h>

#define SHMSZ 64

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

struct sembuf p = {0, -1, SEM_UNDO};
struct sembuf v = {0, +1, SEM_UNDO};

int main() {
    int shmid2, shmid3;
    key_t key2 = 2222, key3 = 3333;
    char *shm2, *shm3;

    // Attach shared memory segments
    if ((shmid2 = shmget(key2, SHMSZ, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((shm2 = shmat(shmid2, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    if ((shmid3 = shmget(key3, sizeof(int), 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((shm3 = shmat(shmid3, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    int id3 = semget(key3, 1, 0666);
    if (id3 < 0) {
        perror("semget");
        exit(1);
    }

    while (true) {
        if (semop(id3, &p, 1) < 0) {
            perror("semop p");
            exit(1);
        }
        int pos = *shm3;
        if (pos >= 64) {
            break;
        }
        fwrite(shm2, 1, pos, stdout);
        fflush(stdout);
        *shm3 = 0;
        if (semop(id3, &v, 1) < 0) {
            perror("semop v");
            exit(1);
        }
        sleep(1);
    }
    return 0;
}
