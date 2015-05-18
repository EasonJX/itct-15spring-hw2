#include "jpeg_decoder.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror(argv[1]);
        return 1;
    }

    MarkerSegment *seg;
    seg = read_segment(fp);
    assert(seg->type == SOI);
    free(seg);
    while (1) {
        seg = read_segment(fp);
        if (seg == NULL) break;
        if (seg->type == EOI) break;
        switch (seg->type) {
        case APP0: parse_APP0(seg->data.APP0); break;
        default:
            ;
        }
        if (seg->data.ptr != NULL) free(seg->data.ptr);
        free(seg);
    }
    fclose(fp);
    return 0;
}
