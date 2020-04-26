#ifndef QUEUE_H
#define QUEUE_H

typedef struct QueueNode{
	void* value;
	struct QueueNode* next;
}QueueNode;

typedef struct Queue{
	QueueNode* head;
	QueueNode* tail;
}Queue;

void enqueue(Queue* queue, void* value);
void* dequeue(Queue* queue);
Queue* createQueue();
void freeQueue(Queue* queue);



#endif /* QUEUE_H */
