#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc, char *argv[])
{
    pid_t process_id;
    int signal_num;

    if (argc != 3)
    {
        fprintf(stderr, "Uso: %s <número_señal> <PID>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    process_id = atoi(argv[2]);
    signal_num = atoi(argv[1]);

    if (kill(process_id, signal_num) == -1)
    {
        perror("Error al enviar la señal");
        exit(EXIT_FAILURE);
    }

    return 0;
}
