#include "jpeg_decoder.h"

Node pool[POOL_SIZE];
int pos;

void huffman_insert(Node **node, int code, int len, uint8_t sym)
{
    if (*node == NULL) {
        *node = &pool[pos++];
    }
    if (len == 0) {
        (*node)->sym = sym;
    } else {
        int c = (code >> (len - 1)) & 1;
        huffman_insert(&(*node)->child[c], code, len - 1, sym);
    }
}
