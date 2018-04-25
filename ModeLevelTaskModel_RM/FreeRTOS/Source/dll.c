#include <stdlib.h>
#include "FreeRTOS.h"
#include "dll.h"

void vDllInitialise(dll *list) {
	list->current = (dllNode *) &(list->tail);
	list->tail.value = portMAX_DELAY;
	list->tail.mode = 0;
	list->tail.next = (dllNode *) &(list->tail);
	list->tail.prev = (dllNode *) &(list->tail);
	list->dllSize = 0;
}

void vDllInsert(dll *list, dllNode *element) {
	volatile dllNode *iterator;
	unsigned int nValue = element->value;
	iterator = (volatile dllNode *) &(list->tail);
	while (iterator->next->value <= nValue) {
		iterator = iterator->next;
	}
	element->next = iterator->next;
	element->next->prev = (volatile dllNode *) element;
	element->prev = iterator;
	iterator->next = (volatile dllNode *) element;
	++(list->dllSize);
}