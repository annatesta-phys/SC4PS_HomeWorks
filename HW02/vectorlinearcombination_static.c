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

	float a = strtof(argv[1], NULL);
	float Xi = strtof(argv[2], NULL);
	float Yi = strtof(argv[3], NULL);
	long int N = (long int)strtol(argv[4], NULL, 10);
	
	PrintInstruction();
	
	printf("a = %.2f\n", a);
	printf("Xi = %.2f\n", Xi);
	printf("Yi = %.2f\n", Yi);
	printf("N =  %ld\n", N);


	// Allocazione statica
	float X[N];
	float Y[N];
	float D[N];

	long int k;
	
	for(k = 0; k < N; k++) {
		X[k] = Xi;
		Y[k] = Yi;
		D[k] = a*X[k] + Y[k];
	}
	
	printf("The linear combination of two vectors of dimensionality N = %ld was successfully computed!\n", N);
	printf("If you don't believe it, let me check that all the entries are equal to the value that you expect:\n");
	float expected = a*Xi + Yi;
	printf("...\n");
	
	// Confronto con tolleranza
	for(k = 0; k < N; k++) {
	    if(fabs(D[k] - expected) > 1e-8) {
		printf("Component number %ld is wrong\n", k);
	    }
	}
	
	printf("Done! The result, for each component i, is Di = %f\n", expected);
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
