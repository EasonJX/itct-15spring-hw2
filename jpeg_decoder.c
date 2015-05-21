#include "jpeg_decoder.h"

JPEGData jpg;

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }
    jpg.fp = fopen(argv[1], "r");
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
        case DQT: parse_DQT(seg->data.DQT); break;
        case SOF0: parse_SOF0(seg->data.SOF0); break;
        case DHT: parse_DHT(seg->data.DHT); break;
        case SOS: parse_SOS(seg->data.SOS); break;
        case COM: parse_COM(seg); break;
        default:
            ;
        }
        if (seg->data.ptr != NULL) free(seg->data.ptr);
        free(seg);
    }
    fclose(jpg.fp);
    return 0;
}
