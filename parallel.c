#include "parallel.h"
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"
#include "block.h"

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

int hasMessages(){
	return 0;
}

void synchronize(){
}

void sendCellArrayToBlock(CellMessage* array, int count, int* block, Parameters* parameters){
	CellMessage first;
	first.x = positionToBlockIndex(block, parameters);
	first.y = count;

	array[0] = first;
	//TODO Actually send message
}

void receiveCellArrayAtBlock(CellMessage* array, int* count, int* block, Parameters* parameters){
	//TODO Actually receive message
	CellMessage first = array[0];
	blockIndexToPosition(block, first.x, parameters);
	*count = first.y;
}

CellMessage* createCellMessage(Parameters* parameters){
	CellMessage* message = malloc(sizeof(*message) * parameters->block_height * parameters->block_width+1);
	return message;
}

void freeCellMessage(CellMessage* message){
	free(message);
}
