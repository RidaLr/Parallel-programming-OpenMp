#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#define n 1000 // size of the table T1D


double *AllocTable(int Tabsize){
	double *tab = (double*) malloc(sizeof(double)*Tabsize);
	return tab;
}

int main(){

	clock_t time0, time1;
	double  t;
	double somme=0.0;
	double *T1D = (double*) malloc(sizeof(double)*n);
	srand(time(NULL));
	omp_set_num_threads(5);
	
	struct timeval time_before, time_after;
	double time_execution;


	// Time before execution.
	 gettimeofday(&time_before, NULL);

	#pragma omp parallel 
	{
		#pragma omp for schedule(dynamic) reduction(+:somme) 
		for(int i = 0 ; i < n ; i++ ){
			T1D[i] = (double)rand()/3.33;
			somme+=T1D[i];
			printf("Thread number %d , somme = [%d]  \n",(somme));
		}
		
	}

	  gettimeofday(&time_after, NULL);
	  time_execution = (time_after.tv_sec - time_before.tv_sec) + (time_after.tv_usec - time_before.tv_usec) / (double)1000000;


	// Show time performance
	fprintf(stdout, " Table size    : %d\n  Execution time    : %f sec.\n", n, time_execution);
	printf(" La somme finale est %d \n ", somme);
	printf(" Temps d'exécution  : %f\n",(omp_get_wtime() - temps_debut));
	
	return 0;
}

/*
 *   1)-Le temps d'exécution de la somme en fonction de la taille du tableau : 
 * 	-----------------------------------------------------------------------
 * |       N = 1000                     |      N = 10                      |
 *	-----------------------------------------------------------------------
 * |    timeExec = 0.125379             |       timeExec = 0.003231        |
 * |    timeCPU  = 0.037771             |       timeCPU  = 0.002035        |
 *  -----------------------------------------------------------------------
 * 
 * 
 *  2)- Le temps d'exécution de la somme en fonction de nombres de threads : 
 *   on fixe la taille du tableau,  n = 1000
 * 
 * 	-------------------------------------------------------------------------------------------
 * | Nombre de threads |       Temps d'exécution            |      Temps CPU                   |
 *	-------------------------------------------------------------------------------------------
 * |    Nbre = 5       |      time = 0.133267               |     time = 0.049930              |
 * |		 	       |                                    |                                  |
 * |-------------------------------------------------------------------------------------------|
 * |    Nbre = 100     |      time = 0.110300               |     time = 0.050368              |
 * |                   |                                    |                                  |
 *  -------------------------------------------------------------------------------------------
 * |    Nbre = 500     |      time = 0.130194               |     time = 0.042061              |
 * |                   |                                    |                                  |
 *  -------------------------------------------------------------------------------------------
 * */
