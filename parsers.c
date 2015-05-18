#include "jpeg_decoder.h"

void parse_APP0(APP0Segment *seg)
{
    assert(strcmp(seg->JFIF, "JFIF") == 0);
    printf("JPEG version %d.%d\n", seg->version[0], seg->version[1]);
}

uint8_t qt_zz_ac[64], qt_zz_dc[64];

void parse_DQT(DQTSegment *seg)
{
    assert(seg->PqTq >> 4 == 0);
    if ((seg->PqTq & 0xf) == 0) {
        memcpy(qt_zz_dc, seg->qt_zz, sizeof(qt_zz_dc));
    } else if ((seg->PqTq & 0xf) == 1) {
        memcpy(qt_zz_ac, seg->qt_zz, sizeof(qt_zz_ac));
    }
    printf("Tq = %d\n", seg->PqTq & 0xf);
    printf("Quantization table:\n");
    uint8_t mat[8][8];
    zigzag_to_mat(seg->qt_zz, mat);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) printf("%d ", mat[i][j]);
        printf("\n");
    }
}

void print_code(int code, int len)
{
    for (int i = len - 1; i >= 0; i--) putchar('0' + ((code >> i) & 1));
}

Node *huf[2][2];

void parse_DHT(DHTSegment *seg)
{
    int Tc = seg->TcTh >> 4, Th = seg->TcTh & 0xf;
    printf("Tc = %d, Th = %d\n", Tc, Th);
    printf("# of length 1-16:");
    for (int i = 0; i < 16; i++) printf(" %d", seg->n_len[i]);
    printf("\n");
    int code = 0, pos = 0;
    for (int len = 1; len <= 16; len++) {
        for (int i = 0; i < seg->n_len[len - 1]; i++) {
            huffman_insert(&huf[Tc][Th], code, len, seg->sym[pos]);
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
