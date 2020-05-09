#ifndef PARALLEL_H
#define PARALLEL_H
#include "cell.h"

typedef struct CellMessage{
	int x;
	int y;
	Cell cell;
}CellMessage;

void parallelBegin();
void parallelEnd();

unsigned int getRank();
unsigned int getTotalRanks();

CellMessage* createCellMessage(Parameters* parameters);
void freeCellMessage(CellMessage* message);

void synchronize();
int hasMessages();
void sendCellArrayToBlock(CellMessage* array, int count, int* block, Parameters* parameters);
void receiveCellArrayAtBlock(CellMessage* array, int* count, int* block, Parameters* parameters);

#endif /* PARALLEL_H */
