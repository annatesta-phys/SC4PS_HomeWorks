#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "daxpy_chunks.h"

double *daxpy_single(double a, const double *X, const double *Y, long int N)
{
    double *D = (double *) malloc(N * sizeof(double));

    for (long int i = 0; i < N; i++) {
        D[i] = a * X[i] + Y[i];
    }
    return D;
}

/* This function builds the D vector chunk by chunk */
double *daxpy_chunked(double a, const double *X, const double *Y,
                       long int N, int chunk_size)
{
    double *D = (double *) malloc(N * sizeof(double));

    long int n_chunks = ceil(N/(long int)chunk_size);
    printf("Ceiled n_chunks: %ld\n",n_chunks);
    
    /* Here below the loop over the chunks index */
    for (long int k = 0; k < n_chunks; k++) {

        long int current_start = k * chunk_size;              /* chunk first element */
        long int current_end   = current_start + chunk_size;            /* chunk last element */

        /* Fix the end of the last chunk if it's lower than chunk_size */
        if (current_end > N) {
            current_end = N;
        }

        /* Internal loop over the vector elements */
        for (long int i = current_start; i < current_end; i++) {
            D[i] = a * X[i] + Y[i];
        }
    }

    return D;
}

double *compute_partial_chunk_sums(const double *D, long int N,
                                    int chunk_size, long int n_chunks)
{
	/* Dynamically allocate the array partial_chunk_sum of dimension n_chunks, 
	* whose elements will be the partial sums of the vector */
    double *partial_chunk_sum = (double *) malloc(n_chunks * sizeof(double));

    for (long int k = 0; k < n_chunks; k++) {

        long int current_start = k * chunk_size;
        long int current_end   = current_start + chunk_size;
        if (current_end > N) current_end = N;
        
	double chunk_sum = 0.0;
        for (long int i = current_start; i < current_end; i++) {
            chunk_sum += D[i];
        }
        partial_chunk_sum[k] = chunk_sum;
    }

    return partial_chunk_sum;
}

/* This function is used to compute the sum of the D vector elements in the single cycle case */
double sum_array(const double *array, long int n)
{
    double total = 0.0;
    for (long int i = 0; i < n; i++) {
        total += array[i];
    }
    return total;
}

int vectors_are_close(const double *A, const double *B, long int N,
                       double tolerance, long int *first_diff_index)
{
    for (long int i = 0; i < N; i++) {
        if (fabs(A[i] - B[i]) > tolerance) {
        	if (first_diff_index != NULL) *first_diff_index = i;
            return 0;
        }
    }
    return 1;
}
