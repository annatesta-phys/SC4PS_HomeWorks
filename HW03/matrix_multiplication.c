#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

float* MatrixProduct(float *A, float *B, long int N);
float* CreateMatrix(long int N);
void PrintInstruction(void);
void CheckCorrectness(float *C, long int N);
int CreateOutputFile(float *A, float *B, float *C, char *filename, long int N);
void BenchmarkMatMulAlgorithm(float *A, float *B, long int N);

int main(int argc, char *argv[]) {
	
	if(argc != 5)
	{
		fprintf(stderr, "Usage: %s a b N fileout\n", argv[0]);
		return 1;
    	}

	float a = strtof(argv[1], NULL);
	float b = strtof(argv[2], NULL);
	long int N = (long int)strtol(argv[3], NULL, 10);
	char *filename = argv[4];
	
	PrintInstruction();
	
	printf("a = %f\n", a);
	printf("b = %f\n", b);
	printf("N =  %ld\n", N);

	float *A = CreateMatrix(N);
	float *B = CreateMatrix(N);

	// Construct matrices
	
	for (long int i = 0; i < N; i++){
		for (long int j = 0; j < N; j++){
		A[i*N + j] = 0.0f;
		B[i*N + j] = 0.0f;
		}
	    }
	for (long int i = 0; i < N; i++) {
		A[i*N + i] = a;
		B[i*N + i] = b;
	}
	    

	// Calcolo il prodotto
	float *C = MatrixProduct(A, B, N);

	printf("Multiplication between NxN matrices (N = %ld) succesfully accomplished!\n", N);

	CreateOutputFile(A, B, C, filename, N);
	
	printf("\n");
	
	CheckCorrectness(C, N);
	printf("\n");
	
	printf("Algorithm timings:\n");
	BenchmarkMatMulAlgorithm(A, B, N);

	// Libera memoria

	free(A);
	free(B);
	free(C);
	return 0;
}

void PrintInstruction(void)
{
	printf("Hi! This is a C program which implements the multiplication between two matrices, A and B, of dimensionality N:\n");
	printf("C = AB, where A = a I, B = b I, with I the identity matrix NxN.\n");
	printf("You inserted the values of the diagonal of A (a), B (b) and of the dimensionality (N), and finally the name of the output file on which the product matrix will be saved.\n");
}

float* CreateMatrix(long int N)
{
	float *M = calloc(N*N, sizeof(float));
	return M;
}

float* MatrixProduct(float *A, float *B, long int N)
{
    float *C = CreateMatrix(N);

    for (long int i = 0; i < N*N; i++) C[i] = 0.0f;

    for (long int i = 0; i < N; i++) {
        for (long int j = 0; j < N; j++) {
             for (long int k = 0; k < N; k++) {
		    C[i*N + j] += A[i*N + k] * B[k*N + j];
	    }
	}
    }

    return C;
}

void CheckCorrectness(float *C, long int N)
{
	printf("We check the correctness of the computed C matrix evaluating (C - C_expected)[i][i], where C_expected = a*b (for diagonal components) is inserted below as input from the user:\n");
	
	// Differenza diretta
	printf("Check 1: is every diagonal component of C exactly equal to the expected result?\n");
	char buffer[100];
    	double expected;
    	printf("Insert the expected value:\n");
    	fgets(buffer, sizeof(buffer), stdin);
    	expected = strtod(buffer, NULL);

	printf("Expected C[i][i] is %f\n", expected);
	
	for(long int k = 0; k < N; k++) {
	    if(C[k*N + k] != expected) {
		printf("Component number %ld is wrong\n", k);
		printf("The (wrong) result for component %ld is C[%ld][%ld] = %f\n", k, k, k, C[k*N + k]);
		break;
	    }
	    else {
	    	if(k<10) printf("The result for component %ld, which is exactly correct, is C[%ld][%ld] = %f\n", k, k, k, C[k*N + k]);
	    }
	}
	printf("Check 1 is over\n");
	printf("\n");
		
	// Confronto con tolleranza
	printf("Check 2: are the expected result and the C diagonal components equal within a certain threshold?\n");
	for(long int k = 0; k < N; k++) {
	    if(fabs(C[k*N + k] - expected) > 1e-6) {
		printf("Component number %ld is wrong\n", k);
		printf("The (wrong) result for component %ld, is C[%ld][%ld] = %f\n", k, k, k, C[k*N + k]);
		break;
	    }
	    else {
	    	if(k<10) printf("The result for component %ld, which is correct within the chosen threshold (10^-6), is C[%ld][%ld] = %f\n", k, k , k, C[k*N + k]);
	    }
	}
	printf("Check 2 is over\n");

}

int CreateOutputFile(float *A, float *B, float *C, char *filename, long int N)
{

	// Apertura file
	FILE *fp = fopen(filename, "w");
	if (fp == NULL) {
	printf("An error occurred in opening the file!\n");
	return 1;
	}

	// Qui scriviamo i risultati
	fprintf(fp, "A = \n");
	for (long int i = 0; i < N; i++) {
	    for (long int j = 0; j < N; j++) {
		fprintf(fp, "%f ", A[i*N + j]);
	    }
	    fprintf(fp, "\n");
	}
	
	fprintf(fp, "\n");
	fprintf(fp, "B = \n");
	for (long int i = 0; i < N; i++) {
	    for (long int j = 0; j < N; j++) {
		fprintf(fp, "%f ", B[i*N + j]);
	    }
	    fprintf(fp, "\n");
	}
	
	fprintf(fp, "\n");
	fprintf(fp, "C = AB = \n");
	for (long int i = 0; i < N; i++) {
	    for (long int j = 0; j < N; j++) {
		fprintf(fp, "%f ", C[i*N + j]);
	    }
	    fprintf(fp, "\n");
	}
	
	fclose(fp);

	printf("\nResults saved in file %s\n", filename);
	
	return 0;
}

void BenchmarkMatMulAlgorithm(float *A, float *B, long int N)
{
    float *C = CreateMatrix(N);

     printf("We want now to check which is the fastest loop order for the matrix multiplication, where C[i][j] = Sum_k A[i][k] B[k][j], i.e., , C matrix entries have indices (i,j) and k is the contracted index.\n");
    printf("To do so, we will repeat the loops with different orderings for a number M of times that you will decide, evaluate their duration and then average on them.\n");
    printf("Please, insert the number M of repetitions (be careful: if the matrix has high dimensionality, it may take a very long time, so choose wisely!):\n");
    char buffer[100];
    fgets(buffer, sizeof(buffer), stdin);
    long int M = strtol(buffer, NULL, 10);    
    printf("The loops will be performed %ld times.\n", M);

    struct timespec t1, t2;

    double totalijk = 0.0;
    double totalkij = 0.0;
    double totalikj = 0.0;
    
    // cache warm-up
    for (int w = 0; w < 3; w++) {
    for (long int i = 0; i < N; i++) {
        for (long int j = 0; j < N; j++) {
            C[i*N + j] = 0.0f;
        }
    }

    for (long int i = 0; i < N; i++)
        for (long int j = 0; j < N; j++)
            for (long int k = 0; k < N; k++)
                C[i*N + j] += A[i*N + k] * B[k*N + j];
    }
    
    // benchmark loop
    for (long int r = 0; r < M; r++)
    {
        // reset identico SEMPRE
        for (long int i = 0; i < N*N; i++) C[i] = 0.0f;

        // ijk
        clock_gettime(CLOCK_MONOTONIC, &t1);

        for (long int i = 0; i < N; i++)
            for (long int j = 0; j < N; j++)
                for (long int k = 0; k < N; k++)
                    C[i*N + j] += A[i*N + k] * B[k*N + j];

        clock_gettime(CLOCK_MONOTONIC, &t2);
        totalijk += (t2.tv_sec - t1.tv_sec) +
                    (t2.tv_nsec - t1.tv_nsec) * 1e-9;

        // reset
        for (long int i = 0; i < N*N; i++) C[i] = 0.0f;

        // kij
        clock_gettime(CLOCK_MONOTONIC, &t1);

        for (long int k = 0; k < N; k++)
            for (long int i = 0; i < N; i++) {
                float a = A[i*N + k];
                for (long int j = 0; j < N; j++)
                    C[i*N + j] += a * B[k*N + j];
            }

        clock_gettime(CLOCK_MONOTONIC, &t2);
        totalkij += (t2.tv_sec - t1.tv_sec) +
                    (t2.tv_nsec - t1.tv_nsec) * 1e-9;
       
        // reset
        for (long int i = 0; i < N*N; i++) C[i] = 0.0f;

        // ikj
        clock_gettime(CLOCK_MONOTONIC, &t1);

        for (long int i = 0; i < N; i++)
            for (long int k = 0; k < N; k++) {
                float a = A[i*N + k];
                for (long int j = 0; j < N; j++)
                    C[i*N + j] += a * B[k*N + j];
            }

        clock_gettime(CLOCK_MONOTONIC, &t2);
        totalikj += (t2.tv_sec - t1.tv_sec) +
                    (t2.tv_nsec - t1.tv_nsec) * 1e-9;
    }

    printf("Elapsed i-j-k: %.15f seconds\n", totalijk/M);
    printf("Elapsed k-i-j: %.15f seconds\n", totalkij/M);
    printf("Elapsed i-k-j: %.15f seconds\n", totalikj/M);
    
    char *filename = "dati.txt";    
    FILE *fp = fopen(filename, "a");  // append!
    if (fp == NULL) {
        printf("Error opening file for benchmark!\n");
        return;
    }
    // Scrittura su file dati.txt
    fprintf(fp, "%ld %.15f %.15f %.15f\n", N, totalijk/M, totalkij/M, totalikj/M);
    fclose(fp);
    
    free(C);
}
