#ifndef __HUFFMAN_H
#define __HUFFMAN_H

#define POOL_SIZE 1024

struct node_t {
    uint8_t sym;
    struct node_t *child[2];
};
typedef struct node_t Node;

void huffman_insert(Node **node, int code, int len, uint8_t sym);

#endif
