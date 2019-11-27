#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#define N 10000 //The table size 

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
	
	int nb_threads = omp_get_max_threads();
	double t0,t1,t2,t3,t4,t5,t6,t7;
	double temps_debut = omp_get_wtime(); //initial time
	
	/****************************Allocation dynamique, parallele*****************************/
	double *T1D = (double*) malloc(sizeof(double)*N);
	
	t0 = omp_get_wtime();
	#pragma omp parallel for 
		for(int i = 0 ; i < N ; i++ ){
			t4 = omp_get_wtime();
			T1D[i] = (double)rand()/3.33;
			printf("Thread number %d fill this portion [%d] of table \n",omp_get_thread_num(),i);
			t5 = omp_get_wtime() - t4;
			printf("La charge du thread [%d] = %f \n",omp_get_thread_num(), (t5/nb_threads));
		}
	t1 = omp_get_wtime() - temps_debut;
	
	/*****************************Allocation static, Parallele******************************/
	
	double temps_debut2=clock(); //initial time
	double TD1[N];
	#pragma omp parallel for 
		for(int i = 0 ; i < N ; i++ ){
			t6 = omp_get_wtime();
			TD1[i] = (double)rand()/RAND_MAX*100.0-0.0;
			printf("Thread number %d fill this portion [%d] of table \n",omp_get_thread_num(),i);
			t7 = omp_get_wtime() - t6;
			printf("La charge du thread [%d] = %f \n",omp_get_thread_num(), (t7/nb_threads));
		}
	t2 = (clock() - temps_debut2) / (double)CLOCKS_PER_SEC;;

	/*****************************Allocation statique, séquentielle************************************************/
	//Sequetial_static();
	double temps_debut1 = clock();
	double table[N];
	for(int i = 0 ; i < N ; i++ ){
		table[i] = (double)rand()/3.33;
		printf("Thread number %d fill this portion [%d] of table \n",omp_get_thread_num(),i); // thread 0  c'est lui qui remplie touteles portions du tableau
	}
	double temps_fin= clock();
	timestatic = (temps_fin - temps_debut1)/ (double)CLOCKS_PER_SEC;
	
	/*******************************Allocation dynamique, séquentielle******************************************/
	//Sequetial_dynamic();
	
	double temps_debut0 = clock();
	
	double *array = (double*) malloc(sizeof(double)*N);
	
	for(int i = 0 ; i < N ; i++ ){
		array[i] = (double)rand()/3.33;
		printf("Thread number %d fill this portion [%d] of table \n",omp_get_thread_num(),i);// thread 0  c'est lui qui remplie touteles portions du tableau
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
 *
 * 
 * Après l'exécution du programme avec les deux allocations des tableaux (dynamique et statique), on a eu les résultats suivants :
 *  
 *                   ------------------------------------------------------------------------------------------------------------
 *                  |                        STATIQUES                     |				      DYNAMIQUES                     |
 *  VERSION 		 ------------------------------------------------------------------------------------------------------------
 *  PARALLELE   	|   Charges                     | Temps d'exécution    |   Charges                      |  Temps d'exécution |
 *	-----------------------------------------------------------------------------------------------------------------------------
 * |    NL = 10     | charges threads = 0.000216    | time = 0.001079 sec  |   charges threads = 0.001137   | time = 0.005684 sec|
 * |    		    | charges processeurs = 0.000539|                      |  charges processeurs =0.002842 |      				 | 
 * |-----------------------------------------------------------------------------------------------------------------------------|
 * |    NL = 1000   | charges threads =	0.034912 sec| time = 0.174558 sec  |  charges threads = 0.024931    | time = 0.124654 sec|
 * |    		    | charges processeurs = 0.087279|                      |  charges processeurs = 0.062327| 		   		     |
 *  -----------------------------------------------------------------------------------------------------------------------------
 * 
 *  
 *                   ------------------------------------------------------------
 *                  |      STATIQUES             |	  DYNAMIQUES                 |
 *  VERSION 		 ------------------------------------------------------------
 *  SEQUENTIELLE   	|   Temps d'exécution        |     Temps d'exécution         |
 *	-----------------------------------------------------------------------------
 * |    NL = 10     | time = 0.000421 sec        |  time = 0.000426 sec          | 
 * |-----------------------------------------------------------------------------|
 * |    NL = 1000   | time = 0.052244 sec        | time = 0.066245  sec          |
 *  -----------------------------------------------------------------------------
 * 
 *  d'après ce tableau on constate que la version séquentielle est plus efficaces en temps d'execution par rapport à la version
 *  parallèle. 
 *  
 *  */
