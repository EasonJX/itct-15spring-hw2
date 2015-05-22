#include "jpeg_codec.h"

JPEGData jpg;
FILE *log_fp;

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }
    log_fp = fopen("jpeg_decoder.log", "w");
    assert(log_fp != NULL);
    jpg.fp = fopen(argv[1], "rb");
    if (jpg.fp == NULL) {
        perror(argv[1]);
        return 1;
    }

    MarkerSegment *seg;
    seg = read_segment();
    assert(seg->type == SOI);
    free(seg);
    while (1) {
        seg = read_segment();
        if (seg == NULL) break;
        if (seg->type == EOI) break;
        switch (seg->type) {
        case APP0: parse_APP0(seg->data.APP0); break;
        case DQT: parse_DQT(seg->data.DQT, seg->len - 2); break;
        case SOF0: parse_SOF0(seg->data.SOF0); break;
        case DHT: parse_DHT(seg->data.DHT, seg->len - 2); break;
        case SOS: parse_SOS(seg->data.SOS); break;
        case COM: parse_COM(seg); break;
        default:
            ;
        }
        if (seg->data.ptr != NULL) free(seg->data.ptr);
        free(seg);
    }
    fclose(jpg.fp);
    fclose(log_fp);
    return 0;
}
