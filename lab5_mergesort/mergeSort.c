#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <omp.h>

int *arr;
int array_size, thread_count;

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

void mergeSort() {
    int i, j, t;

    omp_set_num_threads(thread_count);
    // ref: https://blog.csdn.net/ACM_Fish/article/details/72854549
    // preprocessing merge single value, which can improve the running time
    #pragma omp parallel for
    for (i = 0; i < array_size/2; i++)
    {
        if (arr[i*2] > arr[i*2+1]) {
            t = arr[i*2];
            arr[i*2] = arr[i*2+1];
            arr[i*2+1] = t;
        }
    }

    // i stands for merge range length
    // j stands for the minimum index of two ranges
    for (i = 2; i < array_size; i *= 2) {
        #pragma omp parallel for
        for (j = 0; j < array_size-i; j += i*2) {
            merge(j, j+i, (j+i*2 < array_size ? j+i*2 : array_size));
        }
    }
}

/* generate random numbers for the array */
void randArray()
{
  int i;
  srand(time(NULL));
  for(i = 0; i < array_size;i++){
    arr[i] = rand()%array_size;
  }
}

/* print the array */
void printArray(int *arr, int array_size) 
{ 
  int i;
  for (i = 0; i < array_size; i++)
  {
    printf("%d ",arr[i]);
  }
  printf("\n");
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

    randArray();

    // printf("Given array is \n"); 
    // printArray(arr, array_size); 

    // clock_t start = clock();
    double start = omp_get_wtime();
    mergeSort();
    printf("use %f time\n", ((double)(omp_get_wtime() - start)));
    // printf("use %f time\n", ((double)(clock() - start)/CLOCKS_PER_SEC));
    
    // printf("\nSorted array is \n"); 
    // printArray(arr, array_size); 

    
    free(arr);
    return 0;
}