#ifndef QUEUE_H
#define QUEUE_H
#include "linkedList.h"

typedef struct Queue{
	ListNode* head;
	ListNode* tail;
}Queue;

void enqueue(Queue* queue, void* value);
void* dequeue(Queue* queue);
Queue* createQueue();
void freeQueue(Queue* queue, void freeData(void* data));



#endif /* QUEUE_H */
