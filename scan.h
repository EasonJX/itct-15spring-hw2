#ifndef __SCAN_H
#define __SCAN_H

struct idct_work_t {
    int16_t *b_zz;
    int cid;
    int V, v, vv;
    int H, h, hh;
    struct idct_work_t *next;
};
typedef struct idct_work_t IDCTWork;

void do_scan();

#endif
