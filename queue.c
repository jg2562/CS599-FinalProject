#include "queue.h"
#include <stdlib.h>

void enqueue(Queue* queue, void* value){
	queue->head = appendToList(queue->head, value);

	if (queue->tail == NULL){
		queue->tail = queue->head;
	} else {
		queue->tail = queue->tail->next;
	}
}

void* dequeue(Queue* queue){
	if (queue->tail == NULL){
		return NULL;
	}

	void* value = NULL;
	queue->head = deleteNodeFromList(queue->head, &value, 0);

	if (queue->head == NULL){
		queue->tail = NULL;
	}

	return value;
}

Queue *createQueue() {
  Queue *queue = malloc(sizeof(*queue));
  queue->head = NULL;
  queue->tail = NULL;
  return queue;
}

void freeQueue(Queue* queue, void freeData(void* data)){
	freeList(queue->head, freeData);
}
