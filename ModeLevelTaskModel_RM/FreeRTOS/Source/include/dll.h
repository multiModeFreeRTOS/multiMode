#ifndef DLL_H
#define DLL_H
/* List definition for scheduling */
struct doublyLinkedListNode {
	unsigned int value;
	void *task;
	int mode;
	volatile struct doublyLinkedListNode *prev;
	volatile struct doublyLinkedListNode *next;
};
typedef struct doublyLinkedListNode dllNode;

struct doublyLinkedList {
	volatile dllNode *current;
	volatile dllNode tail;
	int dllSize;
};
typedef struct doublyLinkedList dll;
#endif