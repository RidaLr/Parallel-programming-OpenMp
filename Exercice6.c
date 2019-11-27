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
	double somme = 0.0;
	double mult = 1.0;
	double subtraction = 0.0;
	
	double *T1D = (double*) malloc(sizeof(double)*n);
	srand(time(NULL));
	omp_set_num_threads(5);
	
	struct timeval time_before, time_after;
	double time_execution;
	int i;
	double t0,t1,t2,t3,tmul,tsum, tsub;

	// Time before execution.
	 gettimeofday(&time_before, NULL);

	#pragma omp parallel 
	{
		t0 = omp_get_wtime();
		#pragma omp for schedule(dynamic) reduction(+:somme) private(i)
			for(i = 0 ; i < n ; i++ ){
				T1D[i] = (double)rand()/RAND_MAX*1000-1.0;
				somme+=T1D[i];
				printf("Thread number %d , somme = [%f]  \n",omp_get_thread_num(),(somme));
			}
		tsum = omp_get_wtime() - t0;
		
		t1 = omp_get_wtime();
		#pragma omp for schedule(dynamic) reduction(*:mult) private(i)
			for(i = 0 ; i < n ; i++ ){
				T1D[i] = (double)(rand()/3.33);///RAND_MAX*1000-1.0;
				mult*=T1D[i];
				printf("Thread number %d , mult = [%f]  \n",omp_get_thread_num(),(mult));
			}
		tmul = omp_get_wtime() - t1;
		printf("mult = [%f]  \n", mult);
		
		t2 = omp_get_wtime();
		#pragma omp for schedule(dynamic) reduction(-:subtraction) private(i)
			for(i = 0 ; i < n ; i++ ){
				T1D[i] = (double)(rand()/3.33);///RAND_MAX*1000-1.0;
				subtraction-=T1D[i];
				printf("Thread number %d , substraction = [%f]  \n",omp_get_thread_num(),(subtraction));
			}
		tsub = omp_get_wtime() - t2;
		printf("sub = [%f]  \n", subtraction);
	}
	printf("somme = [%f]  \n",(somme));

	  gettimeofday(&time_after, NULL);
	  time_execution = (time_after.tv_sec - time_before.tv_sec) + (time_after.tv_usec - time_before.tv_usec) / (double)1000000;


	// Show time performance
	fprintf(stdout, " Table size    : %d\n"
	                "Execution time of parallel region  : %f sec.\n"
	                "Execution time of sum      : %f sec.\n"
	                "Execution time of multiplication   : %f sec.\n"
	                "Execution time of subtraction  : %f sec.\n", n, time_execution, tsum, tmul, tsub);
	
	return 0;
}

/*
 *   Le temps d'exécution et la courbe d'execution 
 * 
 * 	--------------------------------------------------------
 * | Nombre de threads |       Temps d'exécution            |  
 * |--------------------------------------------------------|
 * |    Nbre = 10      |      time = 0.017982 sec           | 
 * |                   |      tsum = 0.010679 sec           | 
 * |    		       |      tmul = 0.001689 sec           | 
 * |		 	       |      tsub = 0.000670 sec           |
 * |--------------------------------------------------------|
 * |    Nbre = 100     |      time = 0.030302 sec           | 
 * |                   |      tsum = 0.016032 sec           | 
 * |    		       |      tmul = 0.002010 sec           | 
 * |		 	       |      tsub = 0.000139 sec           |
 * |--------------------------------------------------------|
 * |    Nbre = 1000    |      time = 0.761392 sec           |
 * |                   |      tsum = 0.069444 sec           | 
 * |    		       |      tmul = 0.336072 sec           | 
 * |		 	       |      tsub = 0.134434 sec           |
 *  --------------------------------------------------------
 * */
