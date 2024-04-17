#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define MAXSIZE 128

struct msgbuf
{
    long mtype;
    char mtext[MAXSIZE];
};

void handle_error(char *s)
{
    perror(s);
    exit(1);
}

int main()
{
    int msqid;
    key_t key;
    struct msgbuf rcvbuffer;

    key = 151515;

    if ((msqid = msgget(key, 0606)) < 0)
        handle_error("msgget()");

    while (1)
    {
        if (msgrcv(msqid, &rcvbuffer, MAXSIZE, 0, IPC_NOWAIT) < 0)
        {
            if (errno == ENOMSG)
            {
                break;
            }
            else
            {
                handle_error("msgrcv");
            }
        }
        printf("%s\n", rcvbuffer.mtext);
    }
    exit(0);
}
