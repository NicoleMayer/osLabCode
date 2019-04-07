#include"init.h"

void *reader(int *buffer){
    sem_wait(&rc); // lock in order to modify readcount
    readcount++;
    if(readcount == 1)
        sem_wait(&db); // lock the file, not allowing writing
    sem_post(&rc);

    printf("\nReader Inside..%d\n", *buffer); 

    sem_wait(&rc);
    readcount--;
    if(readcount == 0)
        sem_post(&db); // unlock the file, allowing writing
    sem_post(&rc);
}

