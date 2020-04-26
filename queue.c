#include "queue.h"
#include <stdlib.h>

QueueNode* createQueueNode(void* value);
void freeQueueNode(QueueNode* node);

void enqueue(Queue* queue, void* value){
	QueueNode* node = createQueueNode(value);
	queue->tail->next = node;
	queue->tail = node;
}

void* dequeue(Queue* queue){
	QueueNode* node = queue->head;
	void* value = node->value;
	queue->head = node->next;
	free(node);
	return value;
}

Queue* createQueue(){
	Queue* queue = malloc(sizeof(*queue));
	queue->head = NULL;
	queue->tail = NULL;
	return queue;
}

void freeQueue(Queue* queue){
	QueueNode* node;
	while (queue->head != NULL){
		node = queue->head;
		queue->head = node->next;
		free(node);
	}
}

QueueNode* createQueueNode(void* value){
	QueueNode* node = malloc(sizeof(*node));
	node->next = NULL;
	node->value = value;
	return node;
}

void freeQueueNode(QueueNode* node){
	free(node);
}
