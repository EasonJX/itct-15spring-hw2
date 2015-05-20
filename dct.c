#include "jpeg_decoder.h"

void idct(double *arr, int n)
{
    // O(n^2)
    double *v = (double*)malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) {
        v[i] = 0;
        for (int j = 0; j < n; j++) {
            v[i] += arr[j] * cos(PI * j * (i + 0.5) / n) / (j == 0 ? sqrt(2.0) : 1.0);
        }
        v[i] /= 2.0;
    }
    for (int i = 0; i < n; i++) arr[i] = v[i];
    free(v);
}

void transpose8x8(double mat[8][8])
{
    for (int i = 0; i < 8; i++) {
        for (int j = i + 1; j < 8; j++) {
            double tmp = mat[i][j];
            mat[i][j] = mat[j][i];
            mat[j][i] = tmp;
        }
    }
}

void idct8x8(double mat[8][8])
{
    for (int i = 0; i < 8; i++) idct(mat[i], 8);
    transpose8x8(mat);
    for (int i = 0; i < 8; i++) idct(mat[i], 8);
    transpose8x8(mat);
}
