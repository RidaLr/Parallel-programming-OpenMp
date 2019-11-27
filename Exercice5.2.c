#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#define n 1000000 // size of the table T1D

/*void FillArray(int size,double table[]){
	double temps_debut=omp_get_wtime();
	#pragma omp parallel for 
	for(int i = 0 ; i < size ; i++ ){
		table[i] = (double)rand()/RAND_MAX*1000-1.0;
		printf("Thread number %d \n",omp_get_num_threads());
	}
	printf("Temps d'exécution  : %f\n",(omp_get_wtime() - temps_debut));
}

void print_array(int size,double table[]){
	double temps_debut=omp_get_wtime();
	//size_t size = sizeof(&table) / sizeof(table[0]);
	#pragma omp parallel for 
	for(int i = 0 ; i < size ; i++ ){
		printf("\n table[%d] = %f ", i, table[i]); 
		printf("Thread number %d \n",omp_get_num_threads());
	}
	printf("Temps d'exécution  : %f\n",(omp_get_wtime() - temps_debut));
}

double *AllocArray(int Tabsize){
	double *tab = (double*) malloc(sizeof(double)*Tabsize);
	return tab;
}*/

int main(){

	clock_t time0, time1;
	double  t;
	double somme=0.0;
	double *T1D = (double*) malloc(sizeof(double)*n);
	srand(time(NULL));
	omp_set_num_threads(15);
	
	// Calcul le temps d'execution (temps initial)
	time0 = omp_get_wtime();

	#pragma omp parallel 
	{
		#pragma omp for reduction(+:somme) 
			for(int i = 0 ; i < n ; i++ ){
				T1D[i] = (double)rand()/3.33;
				somme+=T1D[i];
				printf("Thread number %d , somme = [%d]  \n",omp_get_thread_num(),(somme));
			}
		
	}

	// Calcul le temps d'execution (temps final)
	time1 = omp_get_wtime();
	t = (time1 - time0);

	// Impression des resultats
	fprintf(stdout, " La taille du tableau   : %d\n  Temps d'execution   : %f sec.\n", n, t);
	printf(" La somme finale est %f \n ", somme);
	
	return 0;
}

/*
 *   1)-Avec un nombre de threads fixe nbThreads = 15, 
 *   Le temps d'exécution de la somme en fonction de la taille du tableau : 
 *
 * 	-----------------------------------------------------------------------
 * |       N = 1000          |      N = 1000000       |   N = 10000        |
 *	-----------------------------------------------------------------------
 * |    time  = 0.000000 sec |   time = 156.000000sec | time = 2.000000 sec|
 *  -----------------------------------------------------------------------
 * 
 * 
 *  2)- Le temps d'exécution de la somme en fonction de nombres de threads : 
 *   on fixe la taille du tableau n = 10000
 * 
 * 	--------------------------------------------------------
 * | Nombre de threads |       Temps d'exécution            |
 *	--------------------------------------------------------|
 * |    Nbre = 5       |      time = 2.000000 sec           |
 * |--------------------------------------------------------|
 * |    Nbre = 100     |      time = 1.000000 sec           |
 *  --------------------------------------------------------|
 * |    Nbre = 200     |      time = 2.000000 sec           |
 *  --------------------------------------------------------
 * */
