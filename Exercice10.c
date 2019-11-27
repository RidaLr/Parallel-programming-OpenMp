#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

omp_lock_t lock;

double calcul(double* array, int length)
{
	double total = 0.0;
	#pragma omp parallel for
		for ( int i = 0; i < length; i++ )
		{
			omp_set_lock(&lock);
			total+=array[i];
			omp_unset_lock(&lock);
		}
	return total;
}

int main()
{
	
	double array[1024];
	omp_init_lock(&lock);	
	calcul(array,1024);
	omp_destroy_lock(&lock);
}
