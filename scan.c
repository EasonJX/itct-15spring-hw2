#include "jpeg_decoder.h"

extern JPEGData jpg;

uint8_t decode(Node *root, uint8_t *p_byte, int *p_pos)
{
    Node *node = root;
    while (!IS_LEAF(node)) {
        node = huffman_traverse(node, *p_byte, p_pos);
        if (*p_pos == -1) {
            *p_pos = 7;
            uint8_t prev = *p_byte;
            fread(p_byte, 1, 1, jpg.fp);
            if (prev == 0xff && *p_byte == 0) fread(p_byte, 1, 1, jpg.fp);
        }
    }
    return node->sym;
}

uint16_t retrieve(uint8_t *p_byte, int *p_pos, uint8_t len)
{
    uint16_t ret = 0;
    while (len > *p_pos + 1) {
        ret = (ret << (*p_pos + 1)) | (*p_byte & ((1 << (*p_pos + 1)) - 1));
        len -= *p_pos + 1;
        *p_pos = 7;
        uint8_t prev = *p_byte;
        fread(p_byte, 1, 1, jpg.fp);
        if (prev == 0xff && *p_byte == 0) fread(p_byte, 1, 1, jpg.fp);
    }
    ret = (ret << len) | ((*p_byte >> (*p_pos - len + 1)) & ((1 << len) - 1));
    *p_pos -= len;
    return ret;
}

int16_t extend(uint16_t diff, uint8_t len)
{
    int16_t ret = diff;
    if (ret < (1 << (len - 1))) ret -= (1 << len) - 1;
    return ret;
}

void decode_block(int8_t b_zz[64], Component *comp, uint8_t *p_byte, int *p_pos)
{
    memset(b_zz, 0, 64);
    uint8_t len = decode(jpg.huf[0][comp->Td], p_byte, p_pos);
    b_zz[0] = comp->pred + extend(retrieve(p_byte, p_pos, len), len);
    comp->pred = b_zz[0];
    int pos = 1;
    while (pos < 64) {
        len = decode(jpg.huf[1][comp->Ta], p_byte, p_pos);
        if (len == 0) break;
        uint8_t run = HI(len), size = LO(len);
        pos += run;
        b_zz[pos++] = extend(retrieve(p_byte, p_pos, size), size);
    }
    assert(pos <= 64);
    for (pos = 0; pos < 64; pos++) {
        b_zz[pos] = b_zz[pos] * jpg.qt_zz[comp->Tq][pos];
    }
}

void do_scan()
{
    printf("\nStart scanning\n");
    uint8_t byte = 0;
    int pos = -1;
    int cnt_h = (jpg.width - 1) / (jpg.maxH * 8) + 1,
        cnt_v = (jpg.height - 1) / (jpg.maxV * 8) + 1;
    printf("cnt_v = %d, cnt_h = %d\n", cnt_v, cnt_h);
    for (int v = 0; v < cnt_v; v++) for (int h = 0; h < cnt_h; h++) {
        for (int cid = 0; cid < jpg.n_comp; cid++) {
            for (int k = 0; k < jpg.comp[cid].H * jpg.comp[cid].V; k++) {
                int8_t b_zz[64];
                decode_block(b_zz, &jpg.comp[cid], &byte, &pos);
                printf("====\n");
                printf("Component %d, block %d\n", cid, k);
                int8_t mat[8][8];
                zigzag_to_mat(b_zz, mat);
                double fmat[8][8];
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 8; j++) fmat[i][j] = mat[i][j];
                }
                idct8x8(fmat);
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 8; j++) mat[i][j] = round(fmat[i][j]);
                }
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 8; j++) printf("%d ", mat[i][j]);
                    printf("\n");
                }
                for (int i = 0; i < 8 && v*8 + i < jpg.comp[i].height; i++) {
                    for (int j = 0; j < 8 && h*8 + j < jpg.comp[i].width; j++) {
                        jpg.bmp_YCbCr[cid][v*8 + i][h*8 + j] = mat[i][j];
                    }
                }
            }
        }
    }
}
