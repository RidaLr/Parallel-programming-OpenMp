#include <stdio.h>
#include <omp.h>

int i;
//#pragma omp threadprivate(i)
#pragma omp private(i)
int main(){
			
	i=-1;
	//#pragma omp parallel
	//#pragma omp parallel copyin(i)
	#pragma omp private(i)
	{		
		//printf("%d",omp_get_num_procs());//pour afficher le nombre de coeurs 
		printf("La valeur parallèle du thread %d est: %2d\n",omp_get_thread_num(),i);
	}
	printf("La valeur séquentielle est: %2d\n",i);
	return 0;
}

/*
 * 1)- D'après l'exécution du programme, on constate que la notation (threadprivate) permet privatiser la variable i,
 * 		le premier thread de la region parallele a l'accès à la variable i, donc il prend la valeur de i(-1), par contre
 * 		le reste des thread prennent la valeur (0).
 * 2)- D'après l'exécution du programme, on constate que la notation (copyin) permet parteger la variable i,
 * 		tout les threads prennent la meme valeur qui est (-1), c'est à dir qu'ils ont accèes à (i).
 * 3)- D'après l'exécution du programme on constate que ce dernier crée un seul thread, et pas comme les deux précédents.
 * */
