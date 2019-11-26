#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#define NC 1000  // Number of lines
#define NL 900  // Number of columns


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
	double **A = alloc_Matrix(NL,NC); // the matrix A
	double *B = alloc_vector(NL); // the vector that contain the result of A*X
	double *C = alloc_vector(NL); // the vector that contain the result of B + X
	double *D = alloc_vector(NL); // the vector that contain the result of A * C
	double *X = alloc_vector(NL); // the vector X
	double *Y = alloc_vector(NL); // the vector Y = A * ( A * X + X ) + X
	double tmp = 0.0;
	srand(time(NULL));
	omp_set_num_threads(10);
	/****************************************/
	struct timeval time_before, time_after;
	double time_execution;


	// Time before execution.
	 gettimeofday(&time_before, NULL);

	#pragma omp parallel
	{
		// Initialisation of the matrix A and B,C,D,E,X,Y (vectors)
				
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
		
		//Initialisation of the matrix A 
		#pragma omp for schedule(runtime) nowait
			for(int i=0; i < NL; i++) {
			  for(int j=0; j < NC; j++) {
				A[i][j] = (double)rand()/RAND_MAX*100.0-0.0;
			  }
			}
		
		//Initialisation of B
		#pragma omp for schedule(runtime) nowait
			for(int i=0; i < NL; i++) {
				B[i] = 0.0;
			}
		
		//Initialisation of C
		#pragma omp for schedule(runtime)
			for(int i=0; i < NL; i++) {
				C[i] = 0.0;
			}
		
		//Initialisation of D
		#pragma omp for schedule(runtime) nowait
			for(int i=0; i < NL; i++) {
				D[i] = 0.0;
			}
			
		// Product of A*X = B
		#pragma omp for schedule(runtime)
			for(int i=0; i < NL; i++) {
			  for(int j=0; j < NL; j++) {
				  tmp = X[j];
				  B[i] = B[i] + A[i][j] * tmp;
			  }
			}

		// Sum of B+X = C
		#pragma omp for schedule(runtime)
			for(int i=0; i < NL; i++) {
			  C[i] = B[i] + X[i];
			}
			
		// Product of A*C = D
		#pragma omp for schedule(runtime)
			for(int i=0; i < NL; i++) {
			  for(int j=0; j < NL; j++) {
				  D[i] = D[i] + A[i][j] * C[j];
			  }
			}
		
		// Sum of D+X = Y
		#pragma omp for schedule(runtime)
			for(int i=0; i < NL; i++) {
			  Y[i] = D[i] + X[i];
			}
			
		
		//Show  the matrix C = A * B 
		/*#pragma omp for schedule(runtime)
			for(int i = 0 ; i < NL ; i++ ){
					printf("\n C[%d] = %d ", i, C[i]); 
			}*/
				
		
		//Show  the vector  Y = A * X 
		/*#pragma omp for schedule(runtime)
			for(int i = 0 ; i < NL ; i++ ){
				printf(" Y[%d] = %d \t", i, Y[i]); 
			}*/
		
	  } // omp end parallel

	  // Time after execution
	  gettimeofday(&time_after, NULL);
	  time_execution = (time_after.tv_sec - time_before.tv_sec) + (time_after.tv_usec - time_before.tv_usec) / (double)1000000;


	  // Show time performance
	  fprintf(stdout, "\n\n"
		  "   Numbre de lines     : %5d\n"
		  "   Numbre of columns   : %d\n"
		  "   Execution time      : %f sec.\n",
		  NL, NC, time_execution
		  );
		
	free(X);
	free(C);
	free(B);
	desalloc_Matrix(A,NL,NC);
	return 0;
}

/*
 *  On constate que la version séquentielle est mieux que la version parallele en temps d'execution.
 *
 *  					-----------------------------------------------------------------------
 *                     |       Parallele                    |      Sequentielle                |
 *	-------------------------------------------------------------------------------------------
 * |    NL = 96        |      time = 0.007487 sec           |     time = 0.006683 sec          |
 * |	NC = 100       |                                    |                                  |
 * |-------------------------------------------------------------------------------------------|
 * |    NL = 900       |      time = 0.535053 sec           |     time = 0.403712 sec          |
 * |    NC = 1000      |                                    |                                  |
 *  -------------------------------------------------------------------------------------------
 * */
