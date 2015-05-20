#ifndef __HUFFMAN_H
#define __HUFFMAN_H

#define POOL_SIZE 1024
#define IS_LEAF(x) ((x)->child[0] == NULL && (x)->child[1] == NULL)

void huffman_insert(Node **node, int code, int len, uint8_t sym);
Node *huffman_traverse(Node *node, uint8_t byte, int *p_pos);

#endif
