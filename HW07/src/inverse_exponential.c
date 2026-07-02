#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "lcg.h"

#define N_POINTS 500000
#define LAMBDA 1.5

/*
 * Generate samples from the exponential distribution using
 * the inverse transform method:
 *
 *      Y = -ln(1-U)/lambda
 */

static int comp(const void *a, const void *b)
{
    double x = *(const double *)a;
    double y = *(const double *)b;

    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

int main(void)
{
    uint32_t state = 246813579u;

    double *y_exp_array = malloc(N_POINTS * sizeof(double));

    FILE *file = fopen("output/inverse_exp.csv", "w");
    if (file == NULL) {
        fprintf(stderr, "Error: cannot open output/inverse_exp.csv\n");
        free(y_exp_array);
        return 1;
    }

    fprintf(file, "Y, CDF\n");

    /* Generate samples */
    for (int i = 0; i < N_POINTS; ++i) {
        double u = lcg32_next_double(&state);
        y_exp_array[i] = -log(1.0 - u) / LAMBDA;
    }

    /* Sort samples */
    qsort(y_exp_array, N_POINTS, sizeof(double), comp);

    /* Write samples */
    for (int i = 0; i < N_POINTS; ++i) {
        fprintf(file, "%.12f,%.12f\n", y_exp_array[i], (i+1.0)/N_POINTS);
    }

    fclose(file);
    free(y_exp_array);

    printf("Generated %d samples from Exp(lambda = %.2f)\n",
           N_POINTS, LAMBDA);

    return 0;
}
