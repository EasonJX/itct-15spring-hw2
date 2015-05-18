#include "jpeg_decoder.h"

void parse_APP0(APP0Segment *seg)
{
    assert(strcmp(seg->JFIF, "JFIF") == 0);
    printf("JPEG version %d.%d\n", seg->version[0], seg->version[1]);
}

void parse_DQT(DQTSegment *seg)
{
    assert(seg->PqTq >> 4 == 0);
    printf("Tq = %d\n", seg->PqTq & 0xf);
    printf("Quantization table:\n");
    uint8_t mat[8][8];
    zigzag_to_mat(seg->qt_zz, mat);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) printf("%d ", mat[i][j]);
        printf("\n");
    }
}

void parse_DHT(DHTSegment *seg)
{
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
