/*dad_mem_mutex.c*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h> 
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock;

void *mom(){
    int fd;
    printf("Mom comes home.\n");
    sleep(rand()%2+1);
    printf("Mom checks the fridge.\n");

    fd=open("fridge", O_CREAT|O_RDWR|O_APPEND, 0777);
    int state = pthread_mutex_trylock(&lock); //加在check之前比较好
    if(state == 0){
        fd=open("fridge", O_CREAT|O_RDWR|O_APPEND, 0777);
        lseek(fd,0,SEEK_END);
        printf("Mom goes to buy milk...\n");
        sleep(rand()%2+1);
        printf("Mon comes back.\n");
        write(fd,"milk",4);
        printf("Mom puts milk in fridge and leaves.\n");
        pthread_mutex_unlock(&lock);
        close(fd);
    }else{
        pthread_mutex_lock(&lock);
        printf("Mom closes the fridge and leaves.\n");
    }
    pthread_mutex_unlock(&lock);
}

void *dad(){
    int fd;
    printf("Dad comes home.\n");
    sleep(rand()%2+1);
    printf("Dad checks the fridge.\n");
    // pthread_mutex_lock(&lock);
    int state = pthread_mutex_trylock(&lock); //加在check之前比较好
    fd=open("fridge", O_CREAT|O_RDWR|O_APPEND, 0777);
    if(state == 0){
        fd=open("fridge", O_CREAT|O_RDWR|O_APPEND, 0777);
        lseek(fd,0,SEEK_END);
        printf("Dad goes to buy milk...\n");
        sleep(rand()%2+1);
        printf("Das comes back.\n");
        write(fd,"milk",4);
        printf("Dad puts milk in fridge and leaves.\n");
        close(fd);
    }else{
        pthread_mutex_lock(&lock);
        printf("Dad closes the fridge and leaves.\n");
    }
    pthread_mutex_unlock(&lock);
}

int main(int argc, char * argv[]) {
    srand(time(0));
    pthread_t p1, p2;
    int fd = open("fridge", O_CREAT|O_RDWR|O_TRUNC , 0777);  //empty the fridge
    close(fd);
    // Create two threads (both run func)  
    pthread_mutex_init(&lock, PTHREAD_PROCESS_PRIVATE);

    pthread_create(&p1, NULL, mom, NULL); 
    pthread_create(&p2, NULL, dad, NULL); 
  
    // Wait for the threads to end. 
    pthread_join(p1, NULL); 
    pthread_join(p2, NULL); 
    pthread_mutex_destroy(&lock);
}
