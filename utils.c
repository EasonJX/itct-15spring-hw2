#include "jpeg_decoder.h"

void zigzag_to_mat(int16_t zigzag[64], int16_t mat[8][8])
{
    int x = 0, y = 0, dir = 0;
    for (int i = 0; i < 36; i++) {
        mat[x][y] = zigzag[i];
        if (dir == 0) {
            if (x == 0) {
                dir = 1;
                y++;
            } else {
                x--;
                y++;
            }
        } else {
            if (y == 0) {
                dir = 0;
                x++;
            } else {
                x++;
                y--;
            }
        }
    }
    x = 7; y = 1; dir = 0;
    for (int i = 36; i < 64; i++) {
        mat[x][y] = zigzag[i];
        if (dir == 0) {
            if (y == 7) {
                dir = 1;
                x++;
            } else {
                x--;
                y++;
            }
        } else {
            if (x == 7) {
                dir = 0;
                y++;
            } else {
                x++;
                y--;
            }
        }
    }
}
