#include<stdio.h>
#include<stdlib.h>
#include<math.h>

float* VectorLinearCombination(float a, float* X, float* Y, long int N);
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
	
	printf("a = %f\n", a);
	printf("Xi = %f\n", Xi);
	printf("Yi = %f\n", Yi);
	printf("N =  %ld\n", N);


	// Allocazione dinamica
	float *X = (float*) malloc(N * sizeof(float));
	float *Y = (float*) malloc(N * sizeof(float));

	long int k;
	
	for(k = 0; k < N; k++) {
		X[k] = Xi;
		Y[k] = Yi;
	}

	// Calcolo il valore di ogni componente
	float *D = VectorLinearCombination(a, X, Y, N);

	printf("The linear combination of two vectors of dimensionality N = %ld was successfully computed!\n", N);
	printf("If you don't believe it, let me check that all the entries are equal:\n");
	printf("...\n");
	float expected = a*Xi + Yi;
	
	// Confronto con tolleranza
	for(k = 0; k < N; k++) {
		if(fabs(D[k] - expected) > 1e-6){
			 printf("Component number %ld is wrong\n", k);
		}
	}
	printf("Done! The result, for each component i, is Di = %f\n", expected);
	printf("Check is over :)\n");
	
	// Libera memoria
	free(D);
	free(X);
	free(Y);

	return 0;
}

void PrintInstruction(void)
{
	printf("Hi! This is a C program to compute the linear combination of two vectors, with the syntax\n");
	printf("D = a*X + Y, where D, X e Y are vectors of dimensionality N and a is a scalar.\n");
	printf("X and Y's components are all equal to each other.\n");
	printf("You inserted the values of the scalar (a), the vectors' entries (Xi and Yi) and the dimensionality (N).\n");

}

float* VectorLinearCombination(float a, float *X, float *Y, long int N)
{
	float *D = (float*) malloc(N * sizeof(float));
	long int k;
	for(k = 0; k < N; k++) {
		D[k] = a*X[k] + Y[k];
	}
	return D;
}
