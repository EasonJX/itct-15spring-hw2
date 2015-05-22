#include "jpeg_codec.h"

JPEGData jpg;
FILE *log_fp;

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }
    log_fp = fopen("jpeg_encoder.log", "w");
    assert(log_fp != NULL);
    jpg.fp = fopen("output.jpg", "wb");
    assert(jpg.fp != NULL);

    fclose(jpg.fp);
    fclose(log_fp);
    return 0;
}
