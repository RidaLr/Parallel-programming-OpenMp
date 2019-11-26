#include <stdio.h>
#include <omp.h>

int i;
#pragma omp threadprivate(i)
int main(){
			
	i=-1;
	#pragma omp parallel
	{
		#pragma omp single copyprivate(i)
		{i=2;}
		printf("La valeur parallèle est: %2d\n",i);
	}
	printf("La valeur séquentielle est: %2d\n",i);
	return 0;
}
