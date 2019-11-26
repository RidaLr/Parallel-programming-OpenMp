#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>


void remplir(int size,int table[]){
	double temps_debut=omp_get_wtime();
	#pragma omp parallel for 
	for(int i = 0 ; i < size ; i++ ){
		table[i]=rand()%100+1;
		printf("Thread number %d \n",omp_get_num_threads());
	}
	printf("Temps d'exécution  : %f\n",(omp_get_wtime() - temps_debut));
}

void show_table(int n,int table[]){
	double temps_debut=omp_get_wtime();
	size_t size = sizeof(&table) / sizeof(table[0]);
	#pragma omp parallel for 
	for(int i = 0 ; i < n ; i++ ){
		printf("\n table[%d] = %d ", i, table[i]); 
		printf("Thread number %d \n",omp_get_num_threads());
	}
	printf("Temps d'exécution  : %f\n",(omp_get_wtime() - temps_debut));
}

void square(int n,int table[]){
	double temps_debut=omp_get_wtime();
	int c=0;
	size_t size = sizeof(&table) / sizeof(table[0]);
	#pragma omp parallel for 
	for(int i = 0 ; i < n ; i++ ){
		c = table[i] * table[i];
		printf("\n square of %d = %d ", table[i], c); 
		printf("Thread number %d \n",omp_get_num_threads());
	}
	printf("Temps d'exécution  : %f\n",(omp_get_wtime() - temps_debut));
}

int *AllocTable(int Tabsize){
	int *tab = (int*) malloc(sizeof(int)*Tabsize);
	return tab;
}

int main(){
	int n;
	printf("Please enter the size of table ");
	scanf("%d",&n);
	int *tab = AllocTable(n);
	srand(time(NULL));
	omp_set_num_threads(10);
	remplir(n,tab);
	show_table(n,tab);
	square(n,tab);
	
	return 0;
}


/* d)- lors de l'affichage des nombres de threads qui sont crées, on remarque que le numéro de chaque 
 *  thread égale à le nombre de coeurs de la machine.
 * 
 * ************Jeux de test******************
 * Please enter the size of table 5
				Thread number 2 
				Thread number 2 
				Thread number 2 
				Thread number 2 
				Thread number 2 
				Temps d'exécution  : 0.002266

				 table[0] = 24 Thread number 2 

				 table[1] = 94 Thread number 2 

				 table[2] = 92 Thread number 2 

				 table[3] = 25 Thread number 2 

				 table[4] = 48 Thread number 2 
				Temps d'exécution  : 0.001714

				 square of 24 = 576 Thread number 2 

				 square of 94 = 8836 Thread number 2 

				 square of 92 = 8464 Thread number 2 

				 square of 25 = 625 Thread number 2 

				 square of 48 = 2304 Thread number 2 
				Temps d'exécution  : 0.000776
 ***************************************************************
 * 
 * e)- à chaque fois on modifie la valeur donnée à omp_set_num_threads, on constate que le temps d'exécution s'augmente
 *     et le numéro de thread prend le nombre de threads.
 * 
 * -------------------Nombre de threads est 1 ----------------------
 * Please enter the size of table 3
		Thread number 1 
		Thread number 1 
		Thread number 1 
		Temps d'exécution  : 0.000126

		 table[0] = 19 Thread number 1 

		 table[1] = 42 Thread number 1 

		 table[2] = 63 Thread number 1 
		Temps d'exécution  : 0.000323

		 square of 19 = 361 Thread number 1 

		 square of 42 = 1764 Thread number 1 

		 square of 63 = 3969 Thread number 1 
		Temps d'exécution  : 0.000343

 *-----------------Nombre de threads est 5-----------------------
 * Please enter the size of table 3
		Thread number 5 
		Thread number 5 
		Thread number 5 
		Temps d'exécution  : 0.002878

		 table[0] = 87 Thread number 5 

		 table[2] = 79 Thread number 5 

		 table[1] = 36 Thread number 5 
		Temps d'exécution  : 0.001920

		 square of 87 = 7569 Thread number 5 

		 square of 79 = 6241 Thread number 5 

		 square of 36 = 1296 Thread number 5 
		Temps d'exécution  : 0.000857
 *----------------------------------------------------------------
 * --------------------Nombre de threads est 10 -------------------
 * 
 * Please enter the size of table 3
		Thread number 10 
		Thread number 10 
		Thread number 10 
		Temps d'exécution  : 0.002520

		 table[0] = 72 Thread number 10 

		 table[1] = 19 Thread number 10 

		 table[2] = 51 Thread number 10 
		Temps d'exécution  : 0.000595

		 square of 19 = 361 Thread number 10 

		 square of 51 = 2601 Thread number 10 

		 square of 72 = 5184 Thread number 10 
		Temps d'exécution  : 0.001017
 * 
 * ------------------------------------------------------------
 * 
 * f)- on peut modifier le nombre de threads à l'aide de la variable OMP_NUM_THREADS dans le terminal,avec la commande 
 *  export OMP_NUM_THREADS=5
 * */
