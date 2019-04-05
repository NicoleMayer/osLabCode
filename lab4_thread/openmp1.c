#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) 
{
	printf("Parallel area 1: \n");
	
	#pragma omp parallel
	{
		printf("Number of threads = %d\n", omp_get_num_threads());
	  printf("I am thread %d\n", omp_get_thread_num());
	}

	printf("Parallel area 2: \n");
	omp_set_num_threads(8);
	#pragma omp parallel
	{
		printf("Number of threads = %d\n", omp_get_num_threads());
	  printf("I am thread %d\n", omp_get_thread_num());
	}
}