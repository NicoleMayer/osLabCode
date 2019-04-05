#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS	5

struct msg{
  int id;
  char* word; 
};

void PrintHello(struct msg* arg)
{
  printf("%s%d!\n", arg->word, arg->id);
  arg->id=1000;
  pthread_exit(NULL); // thread exit
}

int main(int argc, char *argv[])
{
  pthread_t threads[NUM_THREADS]; // define threads 
  struct msg arg[NUM_THREADS];
  char* word = "Hello World! It's me, thread #";
  for(int t=0;t<NUM_THREADS;t++){
    printf("In main: creating thread %d\n", t);
    arg[t] = (struct msg){t, word};
    pthread_create(&threads[t], NULL, &PrintHello, &arg[t]); // create thread
  }
  for(int t=0;t<NUM_THREADS;t++)
    pthread_join(threads[t], NULL);
  printf("\nall threads finish.\n");
  /* Last thing that main() should do */
  // There is a definite problem if main() finishes before the threads it spawned 
  // if you don't call pthread_exit() explicitly. All of the threads it created will 
  // terminate because main() is done and no longer exists to support the threads.
  // By having main() explicitly call pthread_exit() as the last thing it does, 
  // main() will block and be kept alive to support the threads it created until they are done.

  pthread_exit(NULL);
}