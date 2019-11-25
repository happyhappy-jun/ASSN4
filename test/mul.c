#include <math.h>
#include <time.h>
#include <stdio.h>

int main(){
	clock_t start, end;
	int y, z, i;
	double t1, t2;
	y = 0; 
	z= 0;
	start = clock();
	for(i=0; i<1000000;i++){
		y+= pow(34,2);
	}	
	end = clock();
	t1 = ((double) (end-start))/CLOCKS_PER_SEC;
	start = clock();
	for(i=0; i<1000000; i++){
		z+=34*34;
	}
	end = clock();
	t2 = ((double)(end-start))/CLOCKS_PER_SEC;

	printf("pow: %.8f sec\n", t1);
	printf("mul: %.8f sec\n", t2);
	printf("mul is faster %.2f times\n", t1/t2);
	return 0;
}

