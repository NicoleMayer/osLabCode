#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int array_size;
int thread_count;

int *arr; // array to be sorted

/* chunk for thread to execute */
struct chunk {
    int beg;
    int end;
};

/* merge two subarrays */
void merge(int l, int m, int r)
{
  int i, j, k;
  int len1 = m-l+1;
  int len2 = r-m;
  int *l_arr = malloc(len1 * sizeof(int));
  int *r_arr = malloc(len2 * sizeof(int));

  for (i = 0; i < len1; i++)
    l_arr[i] = arr[l+i];
  for (j = 0; j < len2; j++)
    r_arr[j] = arr[m+1+j];
  i = 0; j = 0; k = l;
  while (i < len1 && j < len2)
  {
    if (l_arr[i] <= r_arr[j])
    {
      arr[k] = l_arr[i];
      i++;
    }
    else
    {
      arr[k] = r_arr[j];
      j++;
    }
    k++;
  }
  while (i < len1)
  {
    arr[k] = l_arr[i];
    i++;
    k++;
  }

  while (j < len2)
  {
    arr[k] = r_arr[j];
    j++;
    k++;
  }

  free(l_arr);
  free(r_arr);
}

/* core function of merge sort */
void mergeSort(int l, int r)
{
  int m;
  if (l < r)
  {
    m = (l + r) / 2;
    mergeSort(l, m);
    mergeSort(m+1, r);
    merge(l, m, r);
  }
}

/* core function of merge sort in multi-thread way */
void * multiThreadMergeSort(void *arg) 
{
    struct chunk *chunk = arg;
    int l = chunk->beg;
    int r = chunk->end;
    mergeSort(l, r);
}

/* generate random numbers for the array */
void randArray(int len, int arr[])
{
  int i;
  srand(time(NULL));
  for(i = 0; i < len;i++){
    arr[i] = rand()%len;
  }
}

/* print the array */
void printArray() 
{ 
  int i;
  for (i = 0; i < array_size; i++)
  {
    printf("%d ",arr[i]);
  }
  printf("\n");
} 

void normalMethod()
{
  mergeSort(0, array_size-1); 
}

void multiThreadMethod()
{
    pthread_t threads[thread_count];
    struct chunk chunklist[thread_count];
    struct chunk *chunk;
    int i;
    int len = array_size / thread_count;
    int base = 0;

    for (i = 0; i < thread_count; i++, base += len) 
    {
        chunk = &chunklist[i];
        chunk->beg = base;
        chunk->end = base+len-1;
        if (i == thread_count - 1)
          chunk->end = array_size-1;
    }

    for (i = 0; i < thread_count; i++)
        pthread_create(&threads[i], NULL, multiThreadMergeSort, &chunklist[i]);

    for (i = 0; i < thread_count; i++)
        pthread_join(threads[i], NULL);

    // merge each thread's result
    chunk = &chunklist[0];
    for (i = 1; i < thread_count; i++) 
    {
        struct chunk *cur_chunk = &chunklist[i];
        merge(chunk->beg, cur_chunk->beg-1, cur_chunk->end);
    }
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
      printf("Usage: ./mergeSort array_size number_of_thread\n");
      return -1;
    }

    if (!sscanf(argv[1], "%d", &array_size) || array_size <= 0)
    {
      printf("Usage: ./mergeSort array_size number_of_thread\narray size should be postive\n");
      return -1;      
    }

    if (!sscanf(argv[2], "%d", &thread_count) || thread_count < 0)
    {
      printf("Usage: ./mergeSort array_size number_of_thread\nthread count is invalid\n");
      return -1;      
    }

    arr = malloc(array_size*sizeof(int));

    randArray(array_size, arr);

    // printf("Given array is \n"); 
    // printArray(); 

    clock_t start = clock();
    if (thread_count == 0)
      normalMethod();
    else
      multiThreadMethod();
    printf("use %f time\n", ((double)(clock() - start)/CLOCKS_PER_SEC));
    
    // printf("\nSorted array is \n"); 
    // printArray(); 

    
    free(arr);
    return 0;
}