#include "jpeg_codec.h"

extern JPEGData jpg;
extern FILE *log_fp;

MarkerType marker_type(uint16_t marker)
{
    switch (marker) {
    case 0xffd8: return SOI;
    case 0xffd9: return EOI;
    case 0xffe0: return APP0;
    case 0xffdb: return DQT;
    case 0xffc0: return SOF0;
    case 0xffc4: return DHT;
    case 0xffda: return SOS;
    case 0xfffe: return COM;
    default: return UNKNOWN;
    }
}

uint16_t type_marker(MarkerType type)
{
    switch (type) {
    case SOI: return 0xffd8;
    case EOI: return 0xffd9;
    case APP0: return 0xffe0;
    case DQT: return 0xffdb;
    case SOF0: return 0xffc0;
    case DHT: return 0xffc4;
    case SOS: return 0xffda;
    case COM: return 0xfffe;
    default: return 0;
    }
}

MarkerSegment *read_segment()
{
    MarkerSegment *seg = (MarkerSegment*)malloc(sizeof(MarkerSegment));
    uint16_t marker;
    fread(&marker, sizeof(uint16_t), 1, jpg.fp);
    marker = econv16(marker);
    seg->type = marker_type(marker);
    switch (seg->type) {
    case SOI:
    case EOI:
        seg->len = 0;
        seg->data.ptr = NULL;
        break;
    case UNKNOWN:
        fprintf(stderr, "unknown marker %x\n", marker);
        free(seg);
        return NULL;
    default:
        fread(&seg->len, sizeof(uint16_t), 1, jpg.fp);
        seg->len = econv16(seg->len);
        seg->data.ptr = malloc(seg->len - 2);
        fread(seg->data.ptr, seg->len - 2, 1, jpg.fp);
    }
    fprintf(log_fp, "read segment, marker = %x, len = %d\n", marker, seg->len);
    return seg;
}

void write_segment(MarkerSegment *seg)
{
    uint16_t marker = type_marker(seg->type);
    uint16_t marker_e = econv16(marker), len_e = econv16(seg->len);
    fwrite(&marker_e, sizeof(uint16_t), 1, jpg.fp);
    switch (seg->type) {
    case SOI:
    case EOI:
        break;
    default:
        fwrite(&len_e, sizeof(uint16_t), 1, jpg.fp);
        fwrite(seg->data.ptr, seg->len - 2, 1, jpg.fp);
    }
    fprintf(log_fp, "write segment, marker = %x, len = %d\n", marker, seg->len);
}
