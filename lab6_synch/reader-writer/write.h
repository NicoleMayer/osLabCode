#include"init.h"

void *writer(int *buffer){
    sem_wait(&db); // wait on semaphore db and decrease it by 1
    (*buffer)++; // write
    printf ("write ::%d\n", *buffer);
    sem_post(&db); // add semaphore db by 1
}

