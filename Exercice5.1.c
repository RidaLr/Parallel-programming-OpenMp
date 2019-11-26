#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#define N 1000 //The table size 

/*
void FillTab(int size,double table[]){
	double temps_debut=omp_get_wtime();
	#pragma omp parallel for 
	for(int i = 0 ; i < size ; i++ ){
		table[i] = (double)rand()/RAND_MAX*2.0-1.0;
		printf("Thread number %d \n",omp_get_num_threads());
	}
	printf("Temps d'exécution  : %f\n",(omp_get_wtime() - temps_debut));
}

void show_table(int n,double table[]){
	double temps_debut=omp_get_wtime();
	//size_t size = sizeof(&table) / sizeof(table[0]);
	#pragma omp parallel for 
	for(int i = 0 ; i < n ; i++ ){
		printf("\n table[%d] = %f ", i, table[i]); 
		printf("Thread number %d \n",omp_get_num_threads());
	}
	printf("Temps d'exécution  : %f\n",(omp_get_wtime() - temps_debut));
}
*/

void Sequetial_static(){
	double temps_debut = clock();
	double table[N];
	for(int i = 0 ; i < N ; i++ ){
		table[i] = (double)rand()/3.33;
		printf("Thread number %d fill this portion [%d] of table \n",omp_get_thread_num(),i);
	}
	double temps_fin= clock();
	printf("Temps d'exécution en séquentielle : %f\n",(temps_fin - temps_debut)/ (double)CLOCKS_PER_SEC);
}


void Sequetial_dynamic(){
	double temps_debut=clock();
	
	double *table = (double*) malloc(sizeof(double)*N);
	
	for(int i = 0 ; i < N ; i++ ){
		table[i] = (double)rand()/3.33;
		printf("Thread number %d fill this portion [%d] of table \n",omp_get_thread_num(),i);
	}
	printf("Temps d'exécution  : %f\n",(clock() - temps_debut)/ (double)CLOCKS_PER_SEC);
}

int main(){
	
	srand(time(NULL));
	omp_set_num_threads(5);
	
	double temps_debut=omp_get_wtime(); //initial time
	
	/****************************Allocation dynamique, parallele*****************************/
	double *T1D = (double*) malloc(sizeof(double)*N);
	
	#pragma omp parallel for 
	for(int i = 0 ; i < N ; i++ ){
		T1D[i] = (double)rand()/3.33;
		printf("Thread number %d fill this portion [%d] of table \n",omp_get_thread_num(),i);
	}
	printf("Temps d'exécution en parallele (allocation dynamique)  : %f\n",(omp_get_wtime() - temps_debut));
	
	/*****************************Allocation static, Parallele******************************/
	
	double temps_debut2=omp_get_wtime(); //initial time
	double TD1[N];
	#pragma omp parallel for 
	for(int i = 0 ; i < N ; i++ ){
		TD1[i] = (double)rand()/RAND_MAX*100.0-0.0;
		printf("Thread number %d fill this portion [%d] of table \n",omp_get_thread_num(),i);
	}
	printf("Temps d'exécution en séquentielle : %f\n",(omp_get_wtime() - temps_debut2));

	/*****************************Allocation statique, séquentielle************************************************/
	Sequetial_static();
	Sequetial_dynamic();
	return 0;
}

/*
 * La charge de chacun des processeurs égale à l'indice du portion du tableau que 
 *
 * 
 * Après l'exécution du programme avec les deux allocations des tableaux (dynamique et statique), on a eu les résultats suivants 
 *					-----------------------------------------------------------------------
 * 	   			   |       Allocation statique          |      Allocation dynamique        |
 *	---------------------------------------------------------------------------------------
 * |En parallèle   |  N = 1000    time = 0.120929       |   N = 1000    time = 0.126435    |
 * |		 	   |  N = 10      time = 0.003602       |   N = 10      time = 0.001694    |
 * |---------------------------------------------------------------------------------------|
 * |En séquentielle|  N = 1000    time = 0.197970       |   N = 1000    time = 0.139411    |
 * |               |  N = 10      time = 0.001173       |   N = 10      time = 0.000635    |
 *  ---------------------------------------------------------------------------------------
 *  
 *  d'après ce tableau on constate que la version séquentielle est plus efficaces pour les tablaux de petites tailles,
 *  par contre la meilleure sollution pour les tableaux qui ont des tailles enormes est la programmation parallèle, 
 *  parceque le temps d'exécution est plus petit que la verssion séquentielle. 
 *  */
