#include "jpeg_decoder.h"

extern JPEGData jpg;

void do_output()
{
    // YCbCr to RGB
    int scale_Cr_x = jpg.maxV / jpg.comp[1].V,
        scale_Cr_y = jpg.maxH / jpg.comp[1].H,
        scale_Cb_x = jpg.maxV / jpg.comp[2].V,
        scale_Cb_y = jpg.maxH / jpg.comp[2].H;
    for (int x = 0; x < jpg.height; x++) {
        for (int y = 0; y < jpg.width; y++) {
            double Y = jpg.bmp_YCbCr[0][x][y],
                   Cr = jpg.bmp_YCbCr[1][MIN(x/scale_Cr_x, jpg.comp[1].height-1)][MIN(y/scale_Cr_y, jpg.comp[1].width-1)],
                   Cb = jpg.bmp_YCbCr[2][MIN(x/scale_Cb_x, jpg.comp[1].height-1)][MIN(y/scale_Cb_y, jpg.comp[2].width-1)];
            int R = round(Y + 1.402 * (Cr - 128.0)),
                G = round(Y - 0.34414 * (Cb - 128.0) - 0.71414 * (Cr - 128.0)),
                B = round(Y + 1.772 * (Cb - 128.0));
            jpg.bmp_RGB[0][x][y] = MAX(MIN(R, 255), 0);
            jpg.bmp_RGB[1][x][y] = MAX(MIN(G, 255), 0);
            jpg.bmp_RGB[2][x][y] = MAX(MIN(B, 255), 0);
        }
    }
    // generate bmp
    bmpfile_t *bmp;
    bmp = bmp_create(jpg.width, jpg.height, 8);
    assert(bmp != NULL);
    for (int x = 0; x < jpg.height; x++) {
        for (int y = 0; y < jpg.width; y++) {
            rgb_pixel_t pixel = {jpg.bmp_RGB[0][x][y], jpg.bmp_RGB[1][x][y], jpg.bmp_RGB[2][x][y]};
            bmp_set_pixel(bmp, y, x, pixel);
        }
    }
    bmp_save(bmp, "output.bmp");
    bmp_destroy(bmp);
}
