#ifndef __UTILS_H
#define __UTILS_H

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

void zigzag_to_mat(int16_t zigzag[64], int16_t mat[8][8]);

#endif
