#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#define n 1000 // size of the table T1D

/*void FillTab(int size,double table[]){
	double temps_debut=omp_get_wtime();
	#pragma omp parallel for 
	for(int i = 0 ; i < size ; i++ ){
		table[i] = (double)rand()/RAND_MAX*1000-1.0;
		printf("Thread number %d \n",omp_get_num_threads());
	}
	printf("Temps d'exécution  : %f\n",(omp_get_wtime() - temps_debut));
}

void print_table(int size,double table[]){
	double temps_debut=omp_get_wtime();
	//size_t size = sizeof(&table) / sizeof(table[0]);
	#pragma omp parallel for 
	for(int i = 0 ; i < size ; i++ ){
		printf("\n table[%d] = %f ", i, table[i]); 
		printf("Thread number %d \n",omp_get_num_threads());
	}
	printf("Temps d'exécution  : %f\n",(omp_get_wtime() - temps_debut));
}

double *AllocTable(int Tabsize){
	double *tab = (double*) malloc(sizeof(double)*Tabsize);
	return tab;
}*/

int main(){

	clock_t time0, time1;
	double  t;
	double somme=0.0;
	double *T1D = (double*) malloc(sizeof(double)*n);
	srand(time(NULL));
	omp_set_num_threads(5);
	
	// Calcul le temps d'execution (temps initial)
	time0 = clock();

	#pragma omp parallel 
	{
		#pragma omp for schedule(dynamic) reduction(+:somme) 
			for(int i = 0 ; i < n ; i++ ){
				T1D[i] = (double)rand()/3.33;
				somme+=T1D[i];
				printf("Thread number %d , somme = [%d]  \n",(somme));
			}
		
	}

	// Calcul le temps d'execution (temps final)
	time1 = clock();
	t = (time1 - time0) / (double)CLOCKS_PER_SEC;

	// Impression des resultats
	fprintf(stdout, " La taille du tableau   : %d\n  Temps d'execution   : %f sec.\n", n, t);
	printf(" La somme finale est %d \n ", somme);
	
	return 0;
}

/*
 *   1)-Le temps d'exécution de la somme en fonction de la taille du tableau : 
 * 	-----------------------------------------------------------------------
 * |       N = 1000                     |      N = 10                      |
 *	-----------------------------------------------------------------------
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
