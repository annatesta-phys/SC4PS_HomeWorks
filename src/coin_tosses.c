#include <stdint.h>
#include <stdio.h>
#include "lcg.h"
 
#define N_TOT 100000
 
/*
 * Simulate coin tosses with the LCG.
 * A value < 0.5 is called heads, otherwise tails.
 * The program prints the running mean so it can be plotted later.
 */
int main(void) {
    uint32_t state = 123456789u;
    int heads = 0;
 
    FILE *file = fopen("output/coin_tosses.csv", "w");
    if (file == NULL) {
        fprintf(stderr, "Error: cannot open output/coin_tosses.csv\n");
        return 1;
    }
 
    fprintf(file, "N_TOT,n_heads/N_TOT\n");
    for (int i = 1; i <= N_TOT; ++i) {
        double u = lcg32_next_double(&state);
        int toss = (u < 0.5) ? 1 : 0;
        heads += toss;
 
        /* Cast to double: heads/i was integer division and always gave 0 or 1. */
        if (i < 30) fprintf(file, "%d,%.8f\n", i, (double) heads / i);
        if (i > 30 && i % 500 == 0) fprintf(file, "%d,%.8f\n", i, (double) heads / i);
    }
 
    fclose(file);
    printf("Final head fraction = %.10f\n", (double) heads / N_TOT);
    return 0;
}

