#ifndef __DCT_H
#define __DCT_H

#define PI acos(-1)
#define EPS 1e-9

void idct(double *arr, int n);
void transpose8x8(double mat[8][8]);
void idct8x8(double mat[8][8]);

#endif
