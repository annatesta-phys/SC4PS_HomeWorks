#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "config_parser.h"
#include "daxpy_chunks.h"
#include "hdf5_writer.h"

#define SUM_TOLERANCE 1e-9

int main(int argc, char *argv[])
{
    const char *config_filename = (argc >= 2) ? argv[1] : "config.txt";

   /* Reading the parameters from the configuration file */
    ConfigTable config;
    if (config_load(config_filename, &config) != 0) {
        fprintf(stderr, "Failed in reading the configuration file '%s'\n", config_filename);
        return EXIT_FAILURE;
    }

    double a       = config_get_double(&config, "a");
    double Xi      = config_get_double(&config, "Xi");
    double Yi      = config_get_double(&config, "Yi");
    long int N     = config_get_long(&config, "N");
    int chunk_size = config_get_int(&config, "chunk_size");

    char output_file[256];
    config_get_string(&config, "output_file", output_file, sizeof(output_file));

    printf("Parameters have been read from '%s':\n", config_filename);
    printf("  a          = %lf\n", a);
    printf("  Xi         = %lf\n", Xi);
    printf("  Yi         = %lf\n", Yi);
    printf("  N          = %ld\n", N);
    printf("  chunk_size = %d\n", chunk_size);
    printf("  output_file= %s\n\n", output_file);

	/* Small unit test */
    if (N <= 0 || chunk_size <= 0) {
        fprintf(stderr, "Error: N and chunk_size must be positive.\n");
        return EXIT_FAILURE;
    }

    /* Allocating memory for X and Y vectors */
    double *X = (double *) malloc(N * sizeof(double));
    double *Y = (double *) malloc(N * sizeof(double));

    for (long int k = 0; k < N; k++) {
        X[k] = Xi;
        Y[k] = Yi;
    }

    /* Compute D with both the single cycle and chunks procedure */
    double *D_single   = daxpy_single(a, X, Y, N);
    double *D_chunked = daxpy_chunked(a, X, Y, N, chunk_size);

   /* The function above gives 0 if the check is successful, 1 instead */
   long int first_diff = -1;
    int same_vector = vectors_are_close(D_single, D_chunked, N, 0.0, &first_diff);

    printf("First check: D_chunked and D_single are the same vector \n");
    if (same_vector) {
        printf("OK: all the %ld components of vector D are exactly equal.\n\n", N);
    } else {
        printf("Error: the components are diverging, starting from %ld\n", first_diff);
        printf("  D_single[%ld]   = %.15lf\n", first_diff, D_single[first_diff]);
        printf("  D_chunked[%ld] = %.15lf\n\n", first_diff, D_chunked[first_diff]);
    }

    /* Partial and single sums */
    long int n_chunks = ceil(N/chunk_size);
    double *partial_chunk_sum = compute_partial_chunk_sums(D_chunked, N, chunk_size, n_chunks);

    double sum_of_partial_sums = sum_array(partial_chunk_sum, n_chunks);
    double direct_sum          = sum_array(D_single, N);

    printf("Second check: direct sum vs chunked sum \n");
    printf("Number of chunks           = %ld\n", n_chunks);
    printf("Direct sum of D            = %.15lf\n", direct_sum);
    printf("Sum of partial sums of D   = %.15lf\n", sum_of_partial_sums);

    if (fabs(sum_of_partial_sums - direct_sum) <= SUM_TOLERANCE) {
        printf("OK: the two sums coincide within the chosen tolerance (%.1e).\n\n", SUM_TOLERANCE);
    } else {
        printf("ERRORE: the two sums differ of %.15lf, beyond the tolerance.\n\n",
               fabs(sum_of_partial_sums - direct_sum));
    }

    /* Print the first partial sums */
    printf("First partial sums per chunk:\n");
    for (long int c = 0; c < n_chunks && c < 10; c++) {
        printf("  partial_chunk_sum[%ld] = %.15lf\n", c, partial_chunk_sum[c]);
    }
    printf("\n");

    /* Save chunks and partial sums in the HDF5 file */
    printf("Saving the results in the HDF5 file\n");
    if (hdf5_save_results(output_file, D_chunked, N, chunk_size, n_chunks,
                           partial_chunk_sum, a) == 0) {
        printf("HDF5 '%s' correctly written.\n", output_file);
        printf("Structure: /chunks/chunk_0000 ... , /partial_chunk_sum, /D_full\n");
    } else {
        fprintf(stderr, "Error occurred, file HDF5 not written'%s'.\n", output_file);
    }
    
    free(X);
    free(Y);
    free(D_single);
    free(D_chunked);
    free(partial_chunk_sum);

    return EXIT_SUCCESS;
}
