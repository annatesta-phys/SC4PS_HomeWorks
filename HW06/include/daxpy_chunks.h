#ifndef DAXPY_CHUNKS_H
#define DAXPY_CHUNKS_H

/**
 * D = a*X + Y c with single-cycle code
 */
double *daxpy_single(double a, const double *X, const double *Y, long int N);

/**
 * D = a*X + Y evaluated dividing the sum in chunks of dimension chunk_size
 */
double *daxpy_chunked(double a, const double *X, const double *Y,
                       long int N, int chunk_size);

/**
 * Evaluates for every chunk the partial sum
 */
double *compute_partial_chunk_sums(const double *D, long int N,
                                    int chunk_size, long int n_chunks);

/** Sums all the elements of an array of length n */
double sum_array(const double *array, long int n);

/**
 * Compares two vector (component by component) against a tolerance. Gives 1 if they pass this
 * test, 0 otherwise.
 */
int vectors_are_close(const double *A, const double *B, long int N,
                       double tolerance, long int *first_diff_index);

#endif /* DAXPY_CHUNKS_H */
