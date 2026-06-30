#ifndef LEGENDRE_UTILS_H
#define LEGENDRE_UTILS_H

#include <stddef.h>

void  forward_recurrence(double x, int lmax, double *P_forw);
void  forward_reference(long double x, int lmax, long double *P_ref);
void  backward_recurrence(double x, int lmax, double *P_back);

#endif
