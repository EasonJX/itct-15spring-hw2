#include "jpeg_codec.h"

extern JPEGData jpg;
extern FILE *log_fp;

void encode_SOI()
{
    MarkerSegment *seg = (MarkerSegment*)malloc(sizeof(MarkerSegment));
    seg->type = SOI;
    write_segment(seg);
    free(seg);
}

void encode_EOI()
{
    MarkerSegment *seg = (MarkerSegment*)malloc(sizeof(MarkerSegment));
    seg->type = EOI;
    write_segment(seg);
    free(seg);
}

void encode_APP0()
{
    MarkerSegment *seg = (MarkerSegment*)malloc(sizeof(MarkerSegment));
    seg->type = APP0;
    uint8_t data[] = {0x4a, 0x46, 0x49, 0x46, 0x00, 0x01, 0x01, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00};
    seg->data.ptr = data;
    seg->len = sizeof(data) + 2;
    write_segment(seg);
    free(seg);
}

void encode_SOF0()
{
    MarkerSegment *seg = (MarkerSegment*)malloc(sizeof(MarkerSegment));
    seg->type = SOF0;
    write_segment(seg);
    free(seg);
}

void encode_DQT()
{
    MarkerSegment *seg = (MarkerSegment*)malloc(sizeof(MarkerSegment));
    seg->type = DQT;
    seg->len = sizeof(DQTSegment) + 2;
    seg->data.ptr = malloc(sizeof(DQTSegment));
    for (int i = 0; i < 2; i++) {
        seg->data.DQT->PqTq |= LO(i);
        for (int j = 0; j < 64; j++) seg->data.DQT->qt_zz[j] = jpg.qt_zz[i][j];
        write_segment(seg);
    }
    free(seg->data.ptr);
    free(seg);
}

void encode_DHT()
{
    MarkerSegment *seg = (MarkerSegment*)malloc(sizeof(MarkerSegment));
    seg->type = DHT;
    write_segment(seg);
    free(seg);
}

void encode_SOS()
{
    MarkerSegment *seg = (MarkerSegment*)malloc(sizeof(MarkerSegment));
    seg->type = SOS;
    write_segment(seg);
    free(seg);
}
