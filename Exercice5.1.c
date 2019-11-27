#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#define N 10 //The table size 

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
/*
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
}*/

int main()
{
	double timestatic, timedynamic;
	srand(time(NULL));
	omp_set_num_threads(5);
	
	double t0,t1,t2,t3;
	double temps_debut = omp_get_wtime(); //initial time
	
	/****************************Allocation dynamique, parallele*****************************/
	double *T1D = (double*) malloc(sizeof(double)*N);
	
	t0 = omp_get_wtime();
	#pragma omp parallel for 
		for(int i = 0 ; i < N ; i++ ){
			T1D[i] = (double)rand()/3.33;
			printf("Thread number %d fill this portion [%d] of table \n",omp_get_thread_num(),i);
		}
	t1 = omp_get_wtime() - temps_debut;
	
	/*****************************Allocation static, Parallele******************************/
	
	double temps_debut2=omp_get_wtime(); //initial time
	double TD1[N];
	#pragma omp parallel for 
		for(int i = 0 ; i < N ; i++ ){
			TD1[i] = (double)rand()/RAND_MAX*100.0-0.0;
			printf("Thread number %d fill this portion [%d] of table \n",omp_get_thread_num(),i);
		}
	t2 = omp_get_wtime() - temps_debut2;

	/*****************************Allocation statique, séquentielle************************************************/
	//Sequetial_static();
	double temps_debut1 = clock();
	double table[N];
	for(int i = 0 ; i < N ; i++ ){
		table[i] = (double)rand()/3.33;
		printf("Thread number %d fill this portion [%d] of table \n",omp_get_thread_num(),i);
	}
	double temps_fin= clock();
	timestatic = (temps_fin - temps_debut1)/ (double)CLOCKS_PER_SEC;
	
	/*******************************Allocation dynamique, séquentielle******************************************/
	//Sequetial_dynamic();
	
	double temps_debut0 = clock();
	
	double *array = (double*) malloc(sizeof(double)*N);
	
	for(int i = 0 ; i < N ; i++ ){
		array[i] = (double)rand()/3.33;
		printf("Thread number %d fill this portion [%d] of table \n",omp_get_thread_num(),i);
	}
	timedynamic = (clock() - temps_debut0)/ (double)CLOCKS_PER_SEC;
	
	// Show execution results.
	fprintf(stdout, "\n\n"
	  "   La taille du tableau  : %d \n"
	  "   Le temps d'execution sequentiel (statique) : %f sec. \n"
	  "   Le temps d'execution sequentiel (dynamique): %f sec. \n"
	  "   Le temps d'execution parallele (statique) : %f sec. \n"
	  "   Le temps d'execution parallele (dynamique): %f sec. \n"
	  "   La charge des threads (Statique)  : %f.\n"
	  "   La charge des threads (dynamique)  : %f.\n"
	  "   La charge des processeurs (statique)  : %f.\n"
	  "   La charge des processeurs (dynamique)  : %f.\n",
	  N, timestatic, timedynamic, t2, t1,(t2/omp_get_max_threads()), (t1/omp_get_max_threads()), (t2/omp_get_num_procs()), (t1/omp_get_num_procs()));

	return 0;
}

/*
 * La charge de chacun des processeurs égale à l'indice du portion du tableau que ce dernier rempli.
 *
 * 
 * Après l'exécution du programme avec les deux allocations des tableaux (dynamique et statique), on a eu les résultats suivants 
 *  
 *                   ---------------------------------------------------------------------------------------------------------------
 *                  |                        STATIQUES                      |				      DYNAMIQUES                        |
 *  VERSION 		 ---------------------------------------------------------------------------------------------------------------
 *  PARALLELE   	|   Charges                      | Temps d'exécution    |   Charges                      | Temps d'exécution	|
 *	--------------------------------------------------------------------------------------------------------------------------------
 * |    NL = 1000    | charges threads = 0.006589    | time = 0.032945 sec  |   charges threads = 0.009816   | time = 0.049082 sec  |
 * |    		     | charges processeurs = 0.016473|                      |  charges processeurs =0.024541 |      				|
 * |--------------------------------------------------------------------------------------------------------------------------------|
 * |    NL = 10     | charges threads =	 0.000076    | time = 0.000380 sec  |  charges threads =     | time =  |
 * |    		    | charges processeurs = 0.000190 |                      |  charges processeurs = | 					    |
 *  --------------------------------------------------------------------------------------------------------------------------------
 * 
 *  
 *                   ---------------------------------------------------------------------------------------------------------------
 *                  |                        STATIQUES                      |				      DYNAMIQUES                        |
 *   VERSION		 ---------------------------------------------------------------------------------------------------------------
 *   SEQUENTIELLE	|   Charges                      | Temps d'exécution    |   Charges                      | Temps d'exécution	|
 *	--------------------------------------------------------------------------------------------------------------------------------
 * |    NL = 700    | charges threads =0.125499      | time = 14.192830 sec |  charges threads =  0.151083   | time = 15.308255 sec |
 * |    NC = 560    | charges processeurs =6.533054  |                      |  charges processeurs =7.225250 |      				|
 * |--------------------------------------------------------------------------------------------------------------------------------|
 * |    NL = 1000   | charges threads =	0.276374     | time = 27.484783 sec |  charges threads = 0.264953    | time = 28.557938 sec |
 * |    NC = 800    | charges processeurs = 13.818709|                      |  charges processeurs =13.247638| 					    |
 *  --------------------------------------------------------------------------------------------------------------------------------
 * |    NL = 1100   | charges threads =	0.323663     | time = 30.711679 sec |  charges threads = 0.318080    | time = 33.619428 sec |
 * |    Nc = 900    | charges processeurs =16.183152 |                      |  charges processeurs =15.903982|     				    |
 *  --------------------------------------------------------------------------------------------------------------------------------
 * 
 *  d'après ce tableau on constate que la version séquentielle est plus efficaces pour les tablaux de petites tailles,
 *  par contre la meilleure sollution pour les tableaux qui ont des tailles enormes est la programmation parallèle, 
 *  parceque le temps d'exécution est plus petit que la verssion séquentielle. 
 *  */
