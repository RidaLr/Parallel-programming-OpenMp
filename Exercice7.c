#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#define NC 100
#define NL 96


/* This fuction allocate a matrix dynamically*/
double **alloc_Matrix(int nl,int nc)
{	
	double **mat=(double**)malloc(nl*sizeof(double *));
	for( int i=0;i<nl;i++)
	{
		mat[i]=(double*)malloc(nc*sizeof(double));
	}
	return mat;
}

void desalloc_Matrix(double **mat,int nl,int nc)
{
	for(int i=0;i<=nl;i++)
	{
		
		free(mat[i]);
		
	}
	
	free(mat);
	
	return;
}

/* This fuction allocate a vector dynamically*/
double *alloc_vector(int size)
{	
	double *vect=(double*)malloc(size*sizeof(double));
	return vect;
}

int main()
{

	/************************************************************************************************/
	double **A = alloc_Matrix(NL,NC);
	double **B = alloc_Matrix(NC,NC);
	double **C = alloc_Matrix(NL,NL);
	double *X = alloc_vector(NL);
	double *Y = alloc_vector(NL);
	
	srand(time(NULL));
	omp_set_num_threads(15);
	/****************************************/
	struct timeval time_before, time_after;
	double time_execution;

	
	// Time before execution
	gettimeofday(&time_before, NULL);

	#pragma omp parallel
	{
		// Initialisation of A,B,C matrixes and the vector X
		
		//Initialisation of the vector X
		#pragma omp for schedule(runtime) nowait
			for(int i=0; i < NL; i++) {
				X[i] = (double)rand()/RAND_MAX*100.0-0.0;
			}
		
		//Initialisation of the vector Y
		#pragma omp for schedule(runtime) nowait
			for(int i=0; i < NL; i++) {
				Y[i] = 0.0;
			}
		
		#pragma omp for schedule(runtime) nowait
			for(int i=0; i < NL; i++) {
			  for(int j=0; j < NC; j++) {
				A[i][j] = (double)rand()/RAND_MAX*100.0-0.0;
			  }
			}

		#pragma omp for schedule(runtime) nowait
			for(int i=0; i < NL; i++) {
			  for(int j=0; j < NC; j++) {
				B[i][j] = (double)rand()/RAND_MAX*100.0-0.0;
			  }
			}

		#pragma omp for schedule(runtime)
			for(int i=0; i < NL; i++) {
			  for(int j=0; j < NC; j++) {
				C[i][j] = 0;
			  }
			}

		  // Produit de matrices
		#pragma omp for schedule(runtime)
			for(int i=0; i < NL; i++) {
			  for(int j=0; j < NL; j++) {
				for(int k=0; k < NC; k++) {
				  C[i][j] = C[i][j] + A[i][k] * B[k][j];
				}
			  }
			}

			//Show  the matrix C = A * B 
			/*#pragma omp for schedule(runtime)
				for(int i = 0 ; i < NL ; i++ ){
					for(int j =0 ; j < NC ; j++){
						printf("\n C[%d][%d] = %d ", i, j, C[i][j]); 
					}
				}*/
			
			  // Product of matrice & vector
			#pragma omp for schedule(runtime)
				for(int i=0; i < NL; i++) {
				  for(int j=0; j < NL; j++) {
					 Y[i] = Y[i] + A[i][j] * X[j];
				  }
				}
				
			
			//Show  the vector  Y = A * X 
			/*#pragma omp for schedule(runtime)
				for(int i = 0 ; i < NL ; i++ ){
					printf(" Y[%d] = %d \t", i, Y[i]); 
				}*/
			
		  }

	  // Time after execution
	  gettimeofday(&time_after, NULL);
	  time_execution = (time_after.tv_sec - time_before.tv_sec) + (time_after.tv_usec - time_before.tv_usec) / (double)1000000;

	  
	  // Show execution results.
	  fprintf(stdout, "\n\n"
		  "   Numbre of lines     : %d \n"
		  "   Numbre of columns   : %d \n"
		  "   Execution time      : %f sec.\n",
		  NL, NC, time_execution);
		
	free(X);
	desalloc_Matrix(A,NL,NC);
	desalloc_Matrix(B,NC,NC);
	desalloc_Matrix(C,NL,NL);
	return 0;
}


/*
 *  On constate que la version sequentielle est mieux que la version parallele lorsqu'on utilise des petites tailles,
 * 	par contre si on utilise des grandes tailles, la version parallele est efficace en temps d'execution.
 *
 *  					-----------------------------------------------------------------------
 *                     |       Parallele                    |      Sequentielle                |
 *	-------------------------------------------------------------------------------------------
 * |    NL = 96        |      time = 0.080927 sec           |     time = 0.037374 sec          |
 * |	NC = 100       |                                    |                                  |
 * |-------------------------------------------------------------------------------------------|
 * |    NL = 900       |      time = 28.549089 sec          |     time = 40.522374 sec         |
 * |    NC = 1000      |                                    |                                  |
 *  -------------------------------------------------------------------------------------------
 * */

