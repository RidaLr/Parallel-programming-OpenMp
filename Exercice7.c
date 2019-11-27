#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#define NC 1000
#define NL 900


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
	double time_execution, t0, t1, tmultMat, tmultY, t3,t4;
	
	// Time before execution
	t3 = clock();
	#pragma omp parallel
	{
		// Initialisation of A,B,C matrixes and the vector X
		
		//Initialisation of the vector X
		#pragma omp for schedule(dynamic) nowait
			for(int i=0; i < NL; i++) 
			{
				X[i] = (double)rand()/RAND_MAX*100.0-0.0;
			}
		
		//Initialisation of the vector Y
		#pragma omp for schedule(dynamic) nowait
			for(int i=0; i < NL; i++) 
			{
				Y[i] = 0.0;
			}
		//Initialisation of A
		#pragma omp for schedule(dynamic) nowait
			for(int i=0; i < NL; i++) 
			{
			  for(int j=0; j < NC; j++) 
			  {
					A[i][j] = (double)rand()/RAND_MAX*100.0-0.0;
			  }
			}

		#pragma omp for schedule(dynamic) nowait
			for(int i=0; i < NL; i++) 
			{
			  for(int j=0; j < NC; j++) 
			  {
					B[i][j] = (double)rand()/RAND_MAX*100.0-0.0;
			  }
			}

		#pragma omp for schedule(dynamic)
			for(int i=0; i < NL; i++) 
			{
			  for(int j=0; j < NC; j++) 
			  {
					C[i][j] = 0;
			  }
			}

		  // Produit de matrices
		t0 = clock();
		#pragma omp for schedule(dynamic) nowait
			for(int i=0; i < NL; i++) 
			{
			  for(int j=0; j < NL; j++) 
			  {
				for(int k=0; k < NC; k++) 
				{
				  C[i][j] = C[i][j] + A[i][k] * B[k][j];
				}
			  }
			}
		tmultMat = (clock() - t0) / (double)CLOCKS_PER_SEC;
		
			//Show  the matrix C = A * B 
			/*#pragma omp for schedule(runtime)
				for(int i = 0 ; i < NL ; i++ ){
					for(int j =0 ; j < NC ; j++){
						printf("\n C[%d][%d] = %d ", i, j, C[i][j]); 
					}
				}*/
			
			  // Product of matrice & vector
			t1 = clock();
			#pragma omp for schedule(dynamic)
				for(int i=0; i < NL; i++) 
				{
				  for(int j=0; j < NL; j++) 
				  {
					 Y[i] = Y[i] + A[i][j] * X[j];
				  }
				}
			tmultY = (clock() - t1) / (double)CLOCKS_PER_SEC;
			
			//Show  the vector  Y = A * X 
			/*#pragma omp for schedule(runtime)
				for(int i = 0 ; i < NL ; i++ ){
					printf(" Y[%d] = %d \t", i, Y[i]); 
				}*/
			
		  }
	  t4 = (clock() - t3) / (double)CLOCKS_PER_SEC;

	  // Show execution results.
	  fprintf(stdout, "\n\n"
		  "   Numbre of lines             : %d \n"
		  "   Numbre of columns           : %d \n"
		  "   Execution time of y=A*x     : %f sec\n"
		  "   Execution time of C=A*B     : %f sec\n"
		  "   Execution time of program   : %f sec\n",
		  NL, NC, tmultY, tmultMat, t4);
		
	free(X);
	desalloc_Matrix(A,NL,NC);
	desalloc_Matrix(B,NC,NC);
	desalloc_Matrix(C,NL,NL);
	return 0;
}


/*
 *  On constate que la version parallele est plus efficace que la version sequentiele, d'apres les resultats ci-dessous 
 * 	on constate que la version parallele prend la moitié du temps d'execution de la version sequentielle pour s'executer.
 *
 *  					-----------------------------------------------------------------------
 *                     |       Sequential                   |      Parallele                   |
 *	-------------------------------------------------------------------------------------------
 * |    NL = 96        |      time = 0.136491 sec           |     time = 0.076919 sec          |
 * |	NC = 100       |      timemultY = 0.000309 sec      |     timemultY = 0.000650 sec     |
 * |	               |      timemultC = 0.076876 sec      |     timemultC = 0.008793 sec     |
 * |-------------------------------------------------------------------------------------------|
 * |    NL = 900       |      time = 51.907205 sec          |     time = 40.170156 sec         |
 * |    NC = 1000      |      timemultY = 0.020732 sec      |     timemultY = 0.000701 sec     |
 * |	               |      timemultC = 47.656040 sec     |     timemultC = 39.133504 sec    |
 *  -------------------------------------------------------------------------------------------
 * */

