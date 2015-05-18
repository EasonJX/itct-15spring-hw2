#include "jpeg_decoder.h"

void parse_APP0(APP0Segment *seg)
{
    assert(strcmp(seg->JFIF, "JFIF") == 0);
    printf("JPEG version %d.%d\n", seg->version[0], seg->version[1]);
}
