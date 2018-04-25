#ifndef PAIRING_HEAP_H
#define PAIRING_HEAP_H

typedef struct PairingNode {
  unsigned int element;
  struct PairingNode *leftChild;
  struct PairingNode *nextSibling;
  struct PairingNode *prev;
}pnode;

typedef struct PairingHeap {
  pnode *root;
}pheap;

#endif
