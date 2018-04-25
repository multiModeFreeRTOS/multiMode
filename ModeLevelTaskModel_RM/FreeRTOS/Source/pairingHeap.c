#include <stdio.h>
#include "pairingHeap.h"

pnode* merge(pnode *first, pnode *second) {
  if(second == NULL) {
    return first;
  }

  if(second->element > first->element) {
    second->prev = first->prev;
    first->prev = second;
    first->nextSibling = second->leftChild;
    if(first->nextSibling != NULL) {
      first->nextSibling->prev = first;
    }
    second->leftChild = first;
    first = second;
  } else {
    second->prev = first;
    first->nextSibling = second->nextSibling;
    if(first->nextSibling != NULL) {
      first->nextSibling->prev = first;
    }
    second->nextSibling = first->leftChild;
    if(second->nextSibling != NULL) {
      second->nextSibling->prev = second;
    }
    first->leftChild = second;
  }

  return first;
}

pnode* twoPassMerge(pnode *firstSibling) {
  if (firstSibling == NULL || firstSibling->nextSibling == NULL) {
		return firstSibling;
	} else {
		pnode* sibling1 = firstSibling->nextSibling;
		pnode* sibling2 = sibling1->nextSibling;
		firstSibling->nextSibling = NULL;
		sibling1->nextSibling = NULL;
		if (sibling2 != NULL) {
			sibling2->prev = NULL;
		}

		return merge(merge(firstSibling, sibling1), twoPassMerge(sibling2));
	}
}

void pheapInsert(pheap *heap, pnode* node, unsigned int key) {
  node->element = key;
  node->leftChild = NULL;
  node->nextSibling = NULL;
  node->prev = NULL;
  heap->root = merge(node, heap->root);
}

int pheapExtractMax(pheap *heap) {
	if (heap->root == NULL) {
		return -1;
	} else {
		unsigned int max = heap->root->element;
		pnode *child = heap->root->leftChild;
		// vPortFree(heap->root);
		heap->root = twoPassMerge(child);
		return max;
	}
}

//int main() {
//	pnode *node = malloc(sizeof(pnode));
//  pheap heap = {.root = NULL};
//
//  int i;
//  for(i = 1; i <= 15; ++i) {
//      pheapInsert(&heap, node, i);
//  }
//
//  for(i = 1; i <= 15; ++i) {
//      printf("%d\n", pheapExtractMin(&heap));
//  }
//  system("pause" );
//  return 0;
//}
