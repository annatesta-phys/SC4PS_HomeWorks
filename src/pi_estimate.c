#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "lcg.h"

#define N_POINTS 100000

int main(void) {
    uint32_t state_x = 123u;
    uint32_t state_y = 987654321u;
    int inside = 0;
    
    FILE *file = fopen("output/pi_estimate.csv", "w");
    if (file == NULL) {
        fprintf(stderr, "Error: cannot open output/pi_estimate.csv \n");
        return 1;
    }
    fprintf(file, "N_POINTS, absolute error\n");
    
    for (int i = 0; i < N_POINTS; ++i) {
        double x = lcg32_next_double(&state_x);
        double y = lcg32_next_double(&state_y);
        if (x * x + y * y <= 1.0) {
            inside++;
        }
        if (i >= 100 && i % 100 == 0) fprintf(file, "%d,%.8f\n", i, fabs(4*(double)inside / i - M_PI));
    }
    
    fclose(file);

    printf("Final pi_estimate = %.10f\n", 4.0 * inside / N_POINTS);
    return 0;
}
