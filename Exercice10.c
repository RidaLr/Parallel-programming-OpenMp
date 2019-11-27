#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

omp_lock_t lock;

double calcul(double* array, int length)
{
	double total = 0.0;
	double time = omp_get_wtime();
	#pragma omp parallel for
		
		for(int i=0;i<length;i++)
		{
			array[i] = (double)rand()/RAND_MAX*2.0;
			omp_set_lock(&lock);
			total +=array[i];
			omp_unset_lock(&lock);
		}
	printf("Execution time = %f \n", (omp_get_wtime()-time));
	return total;
}

int main()
{
	double array[10000];
	omp_init_lock(&lock);	
	calcul(array,10000);
	omp_destroy_lock(&lock);
	return 0;
}

/*
 *  a)- comparaison avec l'exercice 5.2 :
 *  	Ce programme s'execute rapidement par rapport à l'exercice 5.2, avec un tableau d'une taille de 10000 ce programme prend (0.012075 sec) 
 * 		pour s'executer par contre le programme de l'exercice 5.2 prend (2.00000 sec), donc il est rapide 165 fois que l'exercice 5.2
 * 		alors ce programme est plus efficace.
 * 
 * 	b)- comparaison avec l'exercice 6:
 * 		Ce programme s'execute rapidement par rapport à l'exercice 5.3, avec un tableau d'une taille de 100000 ce programme prend (0.093067 sec) 
 * 		pour s'executer par contre le programme de l'exercice 6 prend (0.199442 sec) pour faire la somme de ce tableau, donc ce programme
 * 		est plus rapide 2 fois que le programme de l'exercice 6, alors ce peogramme est efficace.
 * 
 * 	c)- comparaison avec l'exercice 9:
 * 		Après l'execution de ce programme avec un tableau d'une taille 10000, on constate que ce programme s'execute en 0.010841 sec,
 * 		par contre dans l'exercice 9 utilisant la même taille du tableau on a eu ces resultats (Parallele stadard 0.355527 sec),
 * 		(avec une section critique 0.345910 sec), (avec une opération atomique 0.330924 sec) et (avec la réduction 0.321445 sec),
 * 		donc on constate que le programme de l'exercice 10 est plus efficace et plus rapide 30 fois que toute les versions de 
 *      l'exercice 9. 
 * 
 * */
