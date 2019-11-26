#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#define NL 700  //NL => number of lignes 
#define NC 560  //NC => numbre of columns 



/* This fuction allocate a 2 dimension table dynamically*/
double **alloc_tab2D(int nl,int nc)
{	
	double **tab=(double**)malloc(nl*sizeof(double *));
	for( int i=0;i<nl;i++)
	{
		tab[i]=(double*)malloc(nc*sizeof(double));
	}
	return tab;
}

void desalloc_tab2D(double **tab,int nl,int nc)
{
	for(int i=0;i<=nl;i++)
	{
		free(tab[i]);
	}
	free(tab);
}

int main()
{
	
	srand(time(NULL));
	omp_set_num_threads(5);
	
	double temps_debut = clock(); //initial time
	
	/****************************Allocation dynamique*****************************/
	double **T2D = alloc_tab2D(NL,NC);
	
	#pragma omp parallel for 
		for(int i = 0 ; i < NL ; i++ )
		{
			for(int j = 0 ; j < NC ; j++ )
			{
				T2D[i][j] = (double)rand()/3.33;
				printf("Thread number %d fill this portion [%d][%d] of table \n",omp_get_thread_num(),i,j);
			}
		}
	double timedynamic = ( clock() - temps_debut ) / (double)CLOCKS_PER_SEC;
	
	/*****************************Allocation static******************************/
	double temps_debut2=clock(); //initial time
	double TA2D[NL][NC];
	#pragma omp parallel for 
		for(int i = 0 ; i < NL ; i++ )
		{
			for(int j = 0 ; j < NC ; j++ )
			{
				TA2D[i][j] = (double)rand()/3.33;
				printf("Thread number %d fill this portion [%d][%d] of table \n",omp_get_thread_num(),i,j);
			}
		}
	printf("Temps d'exécution (Tab statique) : %f sec\n",( (clock() - temps_debut2) / (double) CLOCKS_PER_SEC ));
	printf(" Temps d'exécution (Tab dynamique) : %f sec\n",(timedynamic));
	return 0;
}

/****
 * 
 *  1)- Après l'exécution du programme on a eu les résultats ci-dessous , on constate d'après ces derniers que 
 *    la version (tableau dynamique) est plus efficace que la version (tableau statique). 
 * 
 * Temps d'exécution (Tab statique) : 15.847540 sec
 * Temps d'exécution (Tab dynamique) : 10.950348 sec
 * 
 * 
 * *****/
