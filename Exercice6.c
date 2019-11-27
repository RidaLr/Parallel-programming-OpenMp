#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#define n 1000000 // size of the table T1D


double *AllocTable(int Tabsize){
	double *tab = (double*) malloc(sizeof(double)*Tabsize);
	return tab;
}

void sequential(){
	double somme = 0.0;
	double mult = 1.0;
	double subtraction = 0.0;
	
	double *T1D = (double*) malloc(sizeof(double)*n);
	srand(time(NULL));
	
	struct timeval time_before, time_after;
	double time_execution;
	int i;
	double t0,t1,t2,tmul,tsum, tsub;

	//Addition
	t0 = clock();
	for(i = 0 ; i < n ; i++ ){
		T1D[i] = (double)rand()/RAND_MAX*2.0;
		somme+=T1D[i];
	}
	tsum = (clock() - t0)/ (double)CLOCKS_PER_SEC;

	//Multiplication
	t1 = clock();
	
	for(i = 0 ; i < n ; i++ ){
		T1D[i] = (double)(rand()/RAND_MAX*2.0);
		mult*=T1D[i];
	}
	tmul = (clock() - t1)/ (double)CLOCKS_PER_SEC; 
			
	//Substraction
	t2 = clock();
	
	for(i = 0 ; i < n ; i++ ){
		T1D[i] = (double)(rand()/RAND_MAX*2.0);
		subtraction-=T1D[i];
	}
	tsub = (clock() - t2)/ (double)CLOCKS_PER_SEC;


	// Show time performance
	fprintf(stdout, "\n\n-----------------------SEQUENTIAL VERSION-----------------------------------------"
					"\n Table size    					: %d\n"
	                "Execution time of sum      		: %f sec.\n"
	                "Execution time of multiplication   : %f sec.\n"
	                "Execution time of subtraction 		: %f sec.\n"
	                "Sum   								: %f.\n"
	                "Subtraction						: %f.\n"
	                "Multiplication  					: %f.\n", n, tsum, tmul, tsub, somme, subtraction, mult);
}

int main()
{

	double somme = 0.0;
	double mult = 1.0;
	double subtraction = 0.0;
	
	double *T1D = (double*) malloc(sizeof(double)*n);
	srand(time(NULL));
	omp_set_num_threads(300);
	
	struct timeval time_before, time_after;
	double time_execution;
	int i;
	double t0,t1,t2,tmul,tsum, tsub;

	// Time before execution.
	 gettimeofday(&time_before, NULL);

	#pragma omp parallel 
	{
		t0 = omp_get_wtime();
		#pragma omp for schedule(dynamic) reduction(+:somme) private(i) nowait
			for(i = 0 ; i < n ; i++ ){
				T1D[i] = (double)rand()/RAND_MAX*2.0;
				somme+=T1D[i];
				//printf("Thread number %d , somme = [%f]  \n",omp_get_thread_num(),(somme));
			}
		tsum = omp_get_wtime() - t0;// Execution time of reduction(+:somme)
		
		t1 = omp_get_wtime();
		#pragma omp for schedule(dynamic) reduction(*:mult) private(i) nowait
			for(i = 0 ; i < n ; i++ ){
				T1D[i] = (double)(rand()/RAND_MAX*2.0);
				mult*=T1D[i];
				//printf("Thread number %d , mult = [%f]  \n",omp_get_thread_num(),(mult));
			}
		tmul = omp_get_wtime() - t1; // Execution time of reduction(*:mult)
				
		t2 = omp_get_wtime();
		#pragma omp for schedule(dynamic) reduction(-:subtraction) private(i) nowait
			for(i = 0 ; i < n ; i++ ){
				T1D[i] = (double)(rand()/RAND_MAX*2.0);
				subtraction-=T1D[i];
				//printf("Thread number %d , substraction = [%f]  \n",omp_get_thread_num(),(subtraction));
			}
		tsub = omp_get_wtime() - t2; // Execution time of reduction(-:subtraction)
		
	}

	gettimeofday(&time_after, NULL);
	//Execution time of program
	time_execution = (time_after.tv_sec - time_before.tv_sec) + (time_after.tv_usec - time_before.tv_usec) / (double)1000000;


	// Show time performance
	fprintf(stdout, "\n\n---------------------PARALLELE VERSION-------------------------------------------"
					"\n Table size    : %d\n"
	                "Execution time of parallel region  : %f sec.\n"
	                "Execution time of sum      : %f sec.\n"
	                "Execution time of multiplication   : %f sec.\n"
	                "Execution time of subtraction  : %f sec.\n"
	                "Sum   : %f.\n"
	                "Subtraction  : %f.\n"
	                "Multiplication  : %f.\n", n, time_execution, tsum, tmul, tsub, somme, subtraction, mult);
	//Show the performance of sequential version
	sequential();
	
	return 0;
}

/*
 *  Le temps d'exécution et la courbe de performancs 
 *  
 * D'apres ce tableau on constate que les reductions de la multiplication et la soustraction en parallèle sont plus efficace en temps 
 * 	d'exicution que la version sequentielle, par contre la reduction de l'addition prend plus de temps en execution que celle de la 
 * 	version séquentielle.
 * 
 * 	---------------------------------------------------------------------------------------------
 * | Array size        |   Execution time (Parallele)       |  Execution time (sequential)       |
 * |---------------------------------------------------------------------------------------------|
 * |    Nbre = 100     |      tsum = 0.000038 sec           |       tsum = 0.000040 sec			 |
 * |    		       |      tmul = 0.000005 sec           | 		tmul = 0.000039 sec			 |
 * |		 	       |      tsub = 0.000005 sec           |		tsub = 0.000220 sec			 |
 * |---------------------------------------------------------------------------------------------|
 * |    Nbre = 100000  |      tsum = 0.199442 sec           | 		tsum = 0.050738 sec			 |
 * |    		       |      tmul = 0.022735 sec           | 		tmul = 0.042080 sec			 |
 * |		 	       |      tsub = 0.000041 sec           |		tsub = 0.034659 sec			 |
 * |---------------------------------------------------------------------------------------------|	
 * |    Nbre = 1000000 |      tsum = 2.233588 sec           | 		tsum = 0.346525 sec 		 |
 * |    		       |      tmul = 0.005570 sec           | 		tmul = 0.247345 sec			 |
 * |		 	       |      tsub = 0.001827 sec           |		tsub = 0.350468 sec			 |
 *  ---------------------------------------------------------------------------------------------
 * */
