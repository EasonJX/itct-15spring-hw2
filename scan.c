#include "jpeg_decoder.h"

extern JPEGData jpg;

uint8_t decode(Node *root, uint8_t *p_byte, int *p_pos)
{
    Node *node = root;
    while (!IS_LEAF(node)) {
        node = huffman_traverse(node, *p_byte, p_pos);
        if (*p_pos == -1) {
            *p_pos = 7;
            fread(p_byte, 1, 1, jpg.fp);
        }
    }
    return node->sym;
}

uint16_t retrieve(uint8_t *p_byte, int *p_pos, uint8_t len)
{
    uint16_t ret = 0;
    while (len > *p_pos + 1) {
        ret = (ret << (*p_pos + 1)) | (*p_byte & ((1 << (*p_pos + 1)) - 1));
        len -= *p_pos + 1;
        *p_pos = 7;
        fread(p_byte, 1, 1, jpg.fp);
    }
    ret = (ret << len) | ((*p_byte >> (*p_pos - len + 1)) & ((1 << len) - 1));
    *p_pos -= len;
    return ret;
}

int16_t extend(uint16_t diff, uint8_t len)
{
    int16_t ret = diff;
    if (ret < (1 << (len - 1))) ret -= (1 << len) - 1;
    return ret;
}

void do_scan()
{
    printf("\nStart scanning\n");
    uint8_t byte = 0;
    int pos = -1;
    uint8_t len;
    printf("%d\n", len = decode(jpg.huf[0][0], &byte, &pos));
    printf("%d\n", extend(retrieve(&byte, &pos, len), len));
}
