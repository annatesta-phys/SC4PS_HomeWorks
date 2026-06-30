#include "legendre_utils.h"

#include <math.h>
#include <stdlib.h>

void  forward_recurrence(double x, int lmax, double *P_forw)
{
    	P_forw[0] = 1.0;
    	P_forw[1] = x;
	    for (int l = 1; l < lmax; l++)
	    	{
	    		P_forw[l+1] = ((2.0*l +1.0) * x * P_forw[l] - l * P_forw[l-1])/(l+1.0);
	    	}
}

void forward_reference(long double x, int lmax, long double *P_ref)
{
    P_ref[0] = 1.0L;
    P_ref[1] = x;
    for (int l = 1; l < lmax; l++) {
        long double lf = (long double)l;
        P_ref[l + 1] = ((2.0L * lf + 1.0L) * x * P_ref[l] - lf * P_ref[l - 1])
                     / (lf + 1.0L);
    }
}

void backward_recurrence(double x, int lmax, double *P_back)
{
    double *Ptilde = malloc((lmax + 2) * sizeof(double));
    if (Ptilde == NULL) return;

    Ptilde[lmax + 1] = 0.0;
    Ptilde[lmax] = 1.0;

    for (int l = lmax; l >= 1; l--) {
        Ptilde[l - 1] =
            ((2.0 * l + 1.0) * x * Ptilde[l] - (l + 1.0) * Ptilde[l + 1]) / l;
    }

    for (int l = 0; l <= lmax; l++) {
        P_back[l] = Ptilde[l] / Ptilde[0];
    }

    free(Ptilde);
}
