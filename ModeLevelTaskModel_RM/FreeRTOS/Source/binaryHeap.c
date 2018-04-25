#include "binaryHeap.h"

unsigned int* buildmaxheap(unsigned int a[], int n)
{
    int heapsize = n;
    int j;
    for (j = n/2; j >= 0; j--) {
        maxheapify(a, j, heapsize);
    }
    return a;
}

void maxheapify(unsigned int a[], int i, int heapsize)
{
    int temp, largest, left, right;
    left = (2*i+1);
    right = ((2*i)+2);
    if (left >= heapsize)
        return;
    else {
        if (left < (heapsize) && a[left] > a[i])
            largest = left;
        else
            largest = i;
        if (right < (heapsize) && a[right] > a[largest])
            largest = right;
        if (largest != i) {
            temp = a[i];
            a[i] = a[largest];
            a[largest] = temp;
            maxheapify(a, largest, heapsize);
        }
    }
}

void insert(unsigned int a[], unsigned int i, int *n) {
    ++(*n);
    int index = (*n) - 1;
    int parent;

    for(;index;index=parent){
        parent = (index) / 2;
        if(a[parent] >= i) break;
        a[index] = a[parent];
    }
    a[index] = i;
}

unsigned int extractMax(unsigned int a[], int *n) {
    if(*n <= 0) {
        return 0;
    }
    --(*n);
    int retVal = a[0];
    unsigned int tmp = a[*n];
    a[*n] = a[0];
    a[0] = tmp;
    maxheapify(a, 0, *n);
    return retVal;
}
