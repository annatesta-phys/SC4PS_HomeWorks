#include "legendre_utils.h"

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#define lmax 50

static void write_comparison_csv(const char *path, double *P_forw, long double *P_ref, double *P_back, double x)
{ 
    FILE *fp = fopen(path, "w");
    if (!fp) { perror(path); exit(1); }
 
    fprintf(fp, "l,x, forward, reference, backward,"
                "abs_err_forward,rel_err_forward,"
                "abs_err_backward,rel_err_backward\n");
 
    for (int l = 0; l <= lmax; l++) {
		/* Set the reference values */
        long double ref_l   = P_ref[l];
        double      ref_d   = (double)ref_l;

		/* Evaluate absolute errors */
        double abs_err_fwd  = fabs(P_forw[l]  - ref_d);
        double abs_err_back = fabs(P_back[l] - ref_d);
 
        /* Evaluate relative errors; guard against division by ~0 (P_l has zeros) */
        double denom = fabs(ref_d) > 1e-300 ? fabs(ref_d) : 1e-300;
        double rel_err_fwd  = abs_err_fwd  / denom;
        double rel_err_back = abs_err_back / denom;
 
        fprintf(fp, "%d,%.10g,%.17g,%.17Lg,%.17g,%.6e,%.6e,%.6e,%.6e\n",
                l, x, P_forw[l], ref_l, P_back[l],
                abs_err_fwd, rel_err_fwd, abs_err_back, rel_err_back);
    }
 
    fclose(fp);
}


int main(void)
{
	double *P_forward = NULL;
	long double *P_reference = NULL;
	double *P_backward = NULL;
	char path[256];
	
	double x_val[] = {0.1, 0.5, 0.9, 0.99};
	
	size_t n = sizeof(x_val) / sizeof(x_val[0]);

	/* Allocate memory for the Legendre polynomials */
	
	P_forward = malloc((lmax + 1) * sizeof(double));
	if (P_forward == NULL) {
        fprintf(stderr, "allocation failed in legendre_forward\n");
        exit(EXIT_FAILURE);
    	}
    	
    P_reference = malloc((lmax + 1) * sizeof(long double));
	if (P_reference == NULL) {
        fprintf(stderr, "allocation failed in legendre_reference\n");
        exit(EXIT_FAILURE);
    	}
    	
    P_backward = malloc((lmax + 1) * sizeof(double));
	if (P_backward == NULL) {
        fprintf(stderr, "allocation failed in legendre_backward\n");
        exit(EXIT_FAILURE);
    	}
    	
	for(size_t i = 0; i < n; i++){
	
		forward_recurrence(x_val[i], lmax, P_forward);
		forward_reference((long double)x_val[i], lmax, P_reference);
		backward_recurrence(x_val[i], lmax, P_backward);
		snprintf(path, sizeof(path),"output/data_legendre_x%zu.csv", i);
		
		/* Produce a .csv file for each value of x */
		write_comparison_csv(path, P_forward, P_reference, P_backward, x_val[i]);
	}
	printf("Output files data_legendre produced in output/ directory\n");

	/* Free dynamically allocated memory */
	free(P_forward);
	free(P_reference);
	free(P_backward);
	return 0;
}
