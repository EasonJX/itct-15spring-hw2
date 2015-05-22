#include "jpeg_codec.h"

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

Node *huffman_traverse(Node *node, uint8_t byte, int *p_pos)
{
    assert(node != NULL);
    while (*p_pos >= 0 && node->child[(byte >> *p_pos) & 1] != NULL) {
        node = node->child[(byte >> *p_pos) & 1];
        (*p_pos)--;
    }
    return node;
}
