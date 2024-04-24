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

#define MAX_MESSAGE_SIZE     128

void error(char *msg)
{
    perror(msg);
    exit(1);
}

struct message_buffer
{
    long    message_type;
    char    message_text[MAX_MESSAGE_SIZE];
};

void handle_sigint(int sig);
void send_message(int key, int flags, int number, pid_t child_pid);

int signal_on;

int main(void)
{
    srand(time(NULL));
    signal_on = 0;
    signal(SIGINT, handle_sigint);
    pid_t   child_pid;

    if ((child_pid = fork()) == -1)
    {
        perror("fork");
        exit(1);
    }

    if (child_pid == 0) // HIJOOOOOOOOOOOOOOO
    {
        printf("PID DEL HIJO -> %d\n", getpid());
        while (1)
        {
            if (signal_on)
            {
                for (int i = 0; i < 1000; i++)
                {
                    int num = rand() % 99 + 1;
                    if (num > 10)
                    {
                        printf("Soy hijo y generé -> %d\n", num);
                        send_message(151515, 0606, num, child_pid);
                    }
                    else
                    {
                            printf("Soy hijo y generé -> %d\n", num);
                            send_message(101010, 0660, num, child_pid);
                    }
                }
                exit(0);
            }
            else
            {
                pause();
            }
        }
    }
    else
    {
        printf("PID DEL PADRE -> %d\n", getpid());
        while (1)
        {
            if  (signal_on)
            {
                for (int i = 0; i < 1000; i++)
                {
                    int num = rand() % 99 + 1;
                    if (num > 10)
                    {
                        printf("Soy papá y generé -> %d\n", num);
                        send_message(151515, 0606, num, child_pid);
                        
                    }
                    else
                    {
                            printf("Soy papá y generé -> %d\n", num);
                            send_message(101010, 0660, num, child_pid);
                    }
                }
                exit(0);
            }
            else
            {
                pause();
            }
        }
    }

    return 0;
}

void handle_sigint(int sig)
{
    signal_on = 1;
}

void send_message(int key, int flags, int number, pid_t child_pid)
{
    int message_queue_id;
    int message_flags = IPC_CREAT | flags;
    key_t message_key;
    struct message_buffer buffer;
    size_t buffer_length;

    message_key = key;

    if ((message_queue_id = msgget(message_key, message_flags)) < 0)
    {
        error("msgget");
    }

    buffer.message_type = number;
    if (child_pid)
    {
        snprintf(buffer.message_text, MAX_MESSAGE_SIZE, "Numero enviado del padre -> %d", number);
    }
    else
    {
        snprintf(buffer.message_text, MAX_MESSAGE_SIZE, "Numero enviado del hijo -> %d", number);
    }

    buffer_length = strlen(buffer.message_text) + 1;

    if (msgsnd(message_queue_id, &buffer, buffer_length, IPC_NOWAIT) < 0)
    {
        printf ("%d, %ld, %s, %d \n", message_queue_id, buffer.message_type, buffer.message_text, (int) buffer_length);
        error("msgsnd");
    }
}