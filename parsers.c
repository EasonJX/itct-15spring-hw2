#include "jpeg_decoder.h"

extern JPEGData jpg;
extern FILE *log_fp;

void parse_APP0(APP0Segment *seg)
{
    assert(strcmp(seg->JFIF, "JFIF") == 0);
    fprintf(log_fp, "JPEG version %d.%d\n", seg->version[0], seg->version[1]);
}

void parse_DQT(DQTSegment *seg)
{
    int Pq = HI(seg->PqTq), Tq = LO(seg->PqTq);
    assert(Pq == 0);
    memcpy(jpg.qt_zz[Tq], seg->qt_zz, 64);
    for (int i = 0; i < 64; i++) jpg.qt_zz[Tq][i] = seg->qt_zz[i];
    fprintf(log_fp, "Tq = %d\n", Tq);
    fprintf(log_fp, "Quantization table:\n");
    int16_t mat[8][8];
    zigzag_to_mat(jpg.qt_zz[Tq], mat);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) fprintf(log_fp, "%d ", mat[i][j]);
        fprintf(log_fp, "\n");
    }
}

void parse_SOF0(SOF0Segment *seg)
{
    assert(seg->P == 8);
    jpg.height = seg->height = econv16(seg->height);
    jpg.width = seg->width = econv16(seg->width);
    for (int i = 0; i < 3; i++) {
        jpg.bmp_RGB[i] = (uint8_t**)malloc(jpg.height * sizeof(uint8_t*));
        for (int j = 0; j < jpg.height; j++) {
            jpg.bmp_RGB[i][j] = (uint8_t*)malloc(jpg.width);
        }
    }
    fprintf(log_fp, "Resolution: %dx%d\n", seg->width, seg->height);
    jpg.n_comp = seg->n_comp;
    jpg.maxH = jpg.maxV = 0;
    for (int i = 0; i < seg->n_comp; i++) {
        int H = HI(seg->comp[i].HV), V = LO(seg->comp[i].HV);
        jpg.maxH = MAX(jpg.maxH, H);
        jpg.maxV = MAX(jpg.maxV, V);
        jpg.comp[i] = (Component){seg->comp[i].C, H, V, seg->comp[i].Tq};
        fprintf(log_fp, "Component %d: C = %d, H = %d, V = %d, Tq = %d\n",
                i, seg->comp[i].C, H, V, seg->comp[i].Tq);
    }
    for (int i = 0; i < seg->n_comp; i++) {
        jpg.comp[i].width = (int)jpg.width * jpg.comp[i].H / jpg.maxH;
        jpg.comp[i].height = (int)jpg.height * jpg.comp[i].V / jpg.maxV;
        fprintf(log_fp, "Component %d: (x, y) = (%d, %d)\n",
                i, jpg.comp[i].width, jpg.comp[i].height);
        jpg.bmp_YCbCr[i] = (uint8_t**)malloc(jpg.comp[i].height * sizeof(uint8_t*));
        for (int j = 0; j < jpg.comp[i].height; j++) {
            jpg.bmp_YCbCr[i][j] = (uint8_t*)malloc(jpg.comp[i].width);
        }
    }
}

void print_code(int code, int len)
{
    for (int i = len - 1; i >= 0; i--) fputc('0' + ((code >> i) & 1), log_fp);
}

void parse_DHT(DHTSegment *seg)
{
    int Tc = HI(seg->TcTh), Th = LO(seg->TcTh);
    fprintf(log_fp, "Tc = %d, Th = %d\n", Tc, Th);
    fprintf(log_fp, "# of length 1-16:");
    for (int i = 0; i < 16; i++) fprintf(log_fp, " %d", seg->n_len[i]);
    fprintf(log_fp, "\n");
    int code = 0, pos = 0;
    for (int len = 1; len <= 16; len++) {
        for (int i = 0; i < seg->n_len[len - 1]; i++) {
            huffman_insert(&jpg.huf[Tc][Th], code, len, seg->sym[pos]);
            print_code(code, len);
            fprintf(log_fp, ": %d\n", seg->sym[pos]);
            pos++;
            code++;
        }
        if (code != 0) code <<= 1;
    }
}

void parse_SOS(SOSSegment *seg)
{
    for (int i = 0; i < seg->n_comp; i++) {
        int Td = HI(seg->comp[i].TdTa), Ta = LO(seg->comp[i].TdTa);
        assert(jpg.comp[i].C == seg->comp[i].Cs);
        jpg.comp[i].Td = Td;
        jpg.comp[i].Ta = Ta;
        fprintf(log_fp, "Component %d: Cs = %d, Td = %d, Ta = %d\n",
                i, seg->comp[i].Cs, Td, Ta);
    }
    int Ss = seg->comp[seg->n_comp].Cs,
        Se = seg->comp[seg->n_comp].TdTa,
        Ah = HI(seg->comp[seg->n_comp + 1].Cs),
        Al = LO(seg->comp[seg->n_comp + 1].Cs);
    assert(Ss == 0);
    assert(Se == 63);
    assert(Ah == 0);
    assert(Al == 0);
    do_scan();
    do_output();
}

void parse_COM(MarkerSegment *seg)
{
    int slen = seg->len - 2;
    char *buf = (char*)malloc(slen + 1);
    strncpy(buf, seg->data.COM, slen);
    buf[slen] = '\0';
    fputs("=== Start comment ===", log_fp);
    fputs(buf, log_fp);
    fputs("=== End comment ===", log_fp);
    free(buf);
}
