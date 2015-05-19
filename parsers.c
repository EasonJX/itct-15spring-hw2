#include "jpeg_decoder.h"

extern JPEGData jpg;

void parse_APP0(APP0Segment *seg)
{
    assert(strcmp(seg->JFIF, "JFIF") == 0);
    printf("JPEG version %d.%d\n", seg->version[0], seg->version[1]);
}

void parse_DQT(DQTSegment *seg)
{
    int Pq = HI(seg->PqTq), Tq = LO(seg->PqTq);
    assert(Pq == 0);
    memcpy(jpg.qt_zz[Tq], seg->qt_zz, 64);
    printf("Tq = %d\n", Tq);
    printf("Quantization table:\n");
    uint8_t mat[8][8];
    zigzag_to_mat(seg->qt_zz, mat);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) printf("%d ", mat[i][j]);
        printf("\n");
    }
}

void parse_SOF0(SOF0Segment *seg)
{
    assert(seg->P == 8);
    jpg.height = seg->height = be16toh(seg->height);
    jpg.width = seg->width = be16toh(seg->width);
    printf("Resolution: %dx%d\n", seg->width, seg->height);
    for (int i = 0; i < seg->n_comp; i++) {
        int H = HI(seg->comp[i].HV), V = LO(seg->comp[i].HV);
        jpg.comp[seg->comp[i].C] = (Component){H, V, seg->comp[i].Tq};
        printf("Component %d: C = %d, H = %d, V = %d, Tq = %d\n",
                i, seg->comp[i].C, H, V, seg->comp[i].Tq);
    }
}

void print_code(int code, int len)
{
    for (int i = len - 1; i >= 0; i--) putchar('0' + ((code >> i) & 1));
}

void parse_DHT(DHTSegment *seg)
{
    int Tc = HI(seg->TcTh), Th = LO(seg->TcTh);
    printf("Tc = %d, Th = %d\n", Tc, Th);
    printf("# of length 1-16:");
    for (int i = 0; i < 16; i++) printf(" %d", seg->n_len[i]);
    printf("\n");
    int code = 0, pos = 0;
    for (int len = 1; len <= 16; len++) {
        for (int i = 0; i < seg->n_len[len - 1]; i++) {
            huffman_insert(&jpg.huf[Tc][Th], code, len, seg->sym[pos]);
            print_code(code, len);
            printf(": %d\n", seg->sym[pos]);
            pos++;
            code++;
        }
        if (code != 0) code <<= 1;
    }
}

void parse_SOS(SOSSegment *seg)
{
}

void parse_COM(MarkerSegment *seg)
{
    int slen = seg->len - 2;
    char *buf = (char*)malloc(slen + 1);
    strncpy(buf, seg->data.COM, slen);
    buf[slen] = '\0';
    puts("=== Start comment ===");
    puts(buf);
    puts("=== End comment ===");
    free(buf);
}
