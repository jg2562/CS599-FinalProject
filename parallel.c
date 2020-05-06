#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

static Queue* mpiQueue = NULL;
static Queue* metaQueue = NULL;

typedef struct ParallelMeta{
	unsigned int to;
}ParallelMeta;

void parallelBegin(){
	mpiQueue = createQueue();
	metaQueue = createQueue();
}

void parallelEnd(){
	freeQueue(mpiQueue, NULL);
	freeQueue(metaQueue, NULL);
	mpiQueue = NULL;
	metaQueue = NULL;
}

unsigned int getRank(){
	return 0;
}

unsigned int getTotalRanks(){
	return 1;
}

void sendIntMessage(int* message, unsigned int size, unsigned int rank_to){
	enqueue(mpiQueue, message);
}

unsigned int receiveIntMessage(int** message, unsigned int size, unsigned int rank_from){
	*message = dequeue(mpiQueue);

	return size;
}
