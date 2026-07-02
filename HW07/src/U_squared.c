#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include "lcg.h"

#define N_POINTS 500000
/*
 * Generate samples for two change-of-variables examples:
 *   1) y = u^2
 *   2) y = -log(1-u)/lambda
 *
 * The output can be redirected to a text file and plotted in Python.
 */
int main(void) {
    uint32_t state = 246813579u;
    
    FILE *file = fopen("output/U_squared.csv", "w");
    if (file == NULL) {
        fprintf(stderr, "Error: cannot open output/U_squared.csv \n");
        return 1;
    }
    fprintf(file, "U, $Y = U^2$\n");
    
    for (int i = 0; i < N_POINTS; ++i) {
        double u = lcg32_next_double(&state);
        double y_square = u * u;
        
        fprintf(file, "%.12f,%.12f\n", u, y_square);
    }
    fclose(file);

    printf("Distribution $Y = U^2$ has been built\n");

    return 0;
}
