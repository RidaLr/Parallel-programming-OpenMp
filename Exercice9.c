#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#define n 10000  // Size of array


void fillArray(double array[])
{
	
	#pragma omp parallel for 
		for(int i = 0 ; i < n ; i++ )
		{
			array[i]=(double)(rand()/(double)RAND_MAX);
		}
}

double *alloc_array()
{	
	double *array=(double*)malloc(n*sizeof(double));
	return array;
}

void sum(double array[])
{

	int total=0;
	double time = clock();
	//#pragma omp parallel for private(total)
	//#pragma omp parallel for
	#pragma omp parallel for reduction(+:total)
		for(int i = 0 ; i < n ; i++ ){
			//#pragma omp atomic
			//#pragma omp critical  //critical section
			//{
				total += array[i];
			 // }
			printf("\n total  = %d ", total); 
		}
	double time_f = (clock() - time) / (double)CLOCKS_PER_SEC;
	printf("\n Total after for  =  %d\n",total);
	fprintf(stdout, "\n Size of array   : %d\n Execution time  : %f sec.\n", n, time_f);
}

int main()
{

	/************************************************************************************************/
	double *array = alloc_array(); // allocation
	
	srand(time(NULL));
	omp_set_num_threads(10);
	/****************************************/
	
	fillArray(array);
	sum(array);
		
	free(array);
	return 0;
}

/*
 *  a)- A la sortie de la boucle, la variable total vaut le total du tableau, parce que par defaut la variable total est une variable partagée.
 * 
 *  b)- Quand on déclare la variable total comme une variable privée utilisant le pragrma (#pragma omp parallel for private(tital)),
 * 	à la sortie la variable total vaut 0. 	
 * 
 * 	c)- à l'aide de la section critique le programme est plus efficace que la version precedente, en temps d'execution.
 * 
 *  d)- à l'aide de l'operation atomique, on constate que cette version est plus efficace que la version précedente.
 * 
 * 	e)- à l'aide de la clause de reduction, on constate que cette version est plus efficace que le precedent.
 * 
 * 
 *
 *  					----------------------------------------------------------------------------------------------------
 *                     |   Parallele          |  Parallèle ( section critique)| Parallèle (atomic)   |    réduction         |
 *	------------------------------------------------------------------------------------------------------------------------
 * |    n = 100        |  time = 0.005910 sec |    time = 0.005026 sec  	  |	 time = 0.004930 sec | time = 0.004361 sec  |
 * |	               |                      |                               |                      |                      |
 * |------------------------------------------------------------------------------------------------------------------------|
 * |    n = 1000       |  time = 0.034710 sec |    time = 0.030422 sec 		  |  time =	0.027286 sec | time = 0.028033 sec  |				
 * |                   |                      |                               |						 |						|
 *  ------------------------------------------------------------------------------------------------------------------------|
 * |    n = 10000      |  time = 0.355527 sec |    time = 0.345910 sec 		  |  time = 0.330924 sec | time = 0.321445 sec  |
 * |                   |                      |                               |						 |  					|
 *  -------------------------------------------------------------------------------------------------------------------------
 * */
