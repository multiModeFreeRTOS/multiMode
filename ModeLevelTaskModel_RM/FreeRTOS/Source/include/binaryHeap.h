#ifndef BINARYHEAP_H
#define BINARYHEAP_H

/*
Builds the heap of size n from array a.
*/
unsigned int* buildmaxheap(unsigned int a[], int n);

/*
Heapify array a.
*/
void maxheapify(unsigned int a[], int i, int heapsize);

/*
Insert node n.
*/
void insert(unsigned int a[], unsigned int i, int *n);

/*
Extract max-node. Finds and deletes node with max value.
*/
unsigned int extractMax(unsigned int a[], int *n);

#endif // !BINARYHEAP_H
