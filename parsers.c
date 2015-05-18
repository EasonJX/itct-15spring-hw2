#include "jpeg_decoder.h"

void parse_APP0(APP0Segment *seg)
{
    assert(strcmp(seg->JFIF, "JFIF") == 0);
    printf("JPEG version %d.%d\n", seg->version[0], seg->version[1]);
}

void parse_DQT(DQTSegment *seg)
{
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
