#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void PrintInstruction(void);

int main(int argc, char *argv[]) {
	
	if(argc != 5)
	{
		fprintf(stderr, "Usage: %s a Xi Yi N \n", argv[0]);
		return 1;
    	}

	double a = strtod(argv[1], NULL);
	double Xi = strtod(argv[2], NULL);
	double Yi = strtod(argv[3], NULL);
	long int N = (long int)strtol(argv[4], NULL, 10);
	
	PrintInstruction();
	
	printf("a = %lf\n", a);
	printf("Xi = %lf\n", Xi);
	printf("Yi = %lf\n", Yi);
	printf("N =  %ld\n", N);


	// Allocazione statica
	double X[N];
	double Y[N];
	double D[N];

	long int k;
	
	for(k = 0; k < N; k++) {
		X[k] = Xi;
		Y[k] = Yi;
		D[k] = a*X[k] + Y[k];
	}
	
	printf("The linear combination of two vectors of dimensionality N = %ld was successfully computed!\n", N);
	printf("If you don't believe it, let me check that all the entries are equal to the value that you expect:\n");
	printf("...\n");
	
	// Differenza diretta
	printf("First, we check if every component is exactly equal to the expected result\n");
	double expected = a*Xi + Yi;
	
	for(k = 0; k < N; k++) {
	    if(D[k] != expected) {
		printf("Component number %ld is wrong\n", k);
	    }
	}
	printf("Done! The result, for each component i, is Di = %.15lf\n", expected);
	printf("Check is over :)\n");
		
	// Confronto con tolleranza
	printf("As second check, we verify if the expected result and the D vector components  are equal within a certain threshold\n");
	for(k = 0; k < N; k++) {
	    if(fabs(D[k] - expected) > 1e-6) {
		printf("Component number %ld is wrong\n", k);
	    }
	}
	
	printf("Done! The result, for each component i, is Di = %.15lf\n", expected);
	printf("Check is over :)\n");
	return 0;
}

void PrintInstruction(void)
{
	printf("Hi! This is a C program to compute the linear combination of two vectors, with the syntax\n");
	printf("D = a*X + Y, where D, X e Y are vectors of dimensionality N and a is a scalar.\n");
	printf("X and Y's components are all equal to each other.\n");
	printf("You inserted the values of the scalar (a), the vectors' entries (Xi and Yi) and the dimensionality (N).\n");

}
