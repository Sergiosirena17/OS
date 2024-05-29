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
    int shmid1, shmid2, shmid3;
    key_t key1 = 1111, key2 = 2222, key3 = 3333;
    char *shm1, *shm2, *shm3;

    if ((shmid1 = shmget(key1, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((shm1 = shmat(shmid1, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    if ((shmid2 = shmget(key2, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((shm2 = shmat(shmid2, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    if ((shmid3 = shmget(key3, sizeof(int), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((shm3 = shmat(shmid3, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    int id1 = semget(key1, 1, 0666 | IPC_CREAT);
    int id2 = semget(key2, 1, 0666 | IPC_CREAT);
    int id3 = semget(key3, 1, 0666 | IPC_CREAT);
    if (id1 < 0 || id2 < 0 || id3 < 0) {
        perror("semget");
        exit(1);
    }

    union semun u1, u2, u3;
    u1.val = 1; u2.val = 1; u3.val = 1;
    if (semctl(id1, 0, SETVAL, u1) < 0 || semctl(id2, 0, SETVAL, u2) < 0 || semctl(id3, 0, SETVAL, u3) < 0) {
        perror("semctl");
        exit(1);
    }

    if (*shm1 != 'm' && *shm1 != 'M') {
        *shm1 = 'm';
    }
    *shm3 = 0;

    char *s = "abcdefgh";
    int l = strlen(s);
    for (int i = 0; i < l; ++i) {
        if (semop(id1, &p, 1) < 0) {
            perror("semop p");
            exit(1);
        }
        if (*shm1 == 'm') {
            int pos = *shm3;
            shm2[pos] = s[i];
            shm2[pos+1] = s[i];
            *shm1 = 'M';
            *shm3 += 2;
        } else {
            i--;
        }
        if (semop(id1, &v, 1) < 0) {
            perror("semop v");
            exit(1);
        }
        sleep(rand() % 2);
    }
    return 0;
}
