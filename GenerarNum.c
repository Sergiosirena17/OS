#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHMSZ     27
using namespace std;
main()
{
    char c;
    int shmid;
    key_t key;
    char *shm, *s;

    /*
     * We'll name our shared memory segment
     * "5678".
     */
    key = 5678;

    /*
     * Create the segment.
     */
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    /*
     * Now put some things into the memory for the
     * other process to read.
     */
    s = shm;
    int num = 100;
    char lastvalue = " ";
    /*
     * Now read what the server put in the memory.
     */

    while(1)
    { 
        if(*s != lastvalue)
        { 
            *s = num;
            //*s = lastvalue + "," + *s;
            cout << "VALOR LEIDO DEL SM ->" << *s << endl;
            lastvalue = *s;
            num++;
        }
    }

    exit(0);
}
