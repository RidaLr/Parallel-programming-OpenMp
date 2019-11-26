#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#define X 1000


int main(){
			
	int i=-1;

	printf("I'm the main thread no: %2d\n",omp_get_thread_num());
	printf("The value of local variable of main in the sequential region is : %2d\n",i);
	
	omp_set_num_threads(5);
	#pragma omp parallel private(i)
	{
		
		int j;
		i=omp_get_thread_num();
		for(j=0;j<X;j++){
			printf("The value of local variable of main in the parallel region is : %2d\n",i);
		}
	}
	printf("I'm the thread main no : %2d\n",omp_get_thread_num());
	printf("The value of local variable of main in the sequential region is : %2d\n",i);
	return 0;
}
