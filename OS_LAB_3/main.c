#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

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

int main()
{
    int message_queue_id;
    key_t key;
    struct message_buffer receive_buffer;

    key = 101010;

    if ((message_queue_id = msgget(key, 0660)) < 0)
        error("msgget()");

    for (int i = 1; i <= 10; i += 2)
    {
        while (1)
        {
            if (msgrcv(message_queue_id, &receive_buffer, MAX_MESSAGE_SIZE, i, IPC_NOWAIT) < 0)
            {
                if (errno == ENOMSG)
                {
                    break;
                }
                else
                {
                    error("msgrcv");
                }
            }
            printf("%s\n", receive_buffer.message_text);
        }
    }
    exit(0);
}