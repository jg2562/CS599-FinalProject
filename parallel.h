#ifndef PARALLEL_H
#define PARALLEL_H
#include "cell.h"
#include "cellMap.h"
#include "model.h"

typedef struct CellMessage{
	int x;
	int y;
	Cell cell;
}CellMessage;

void parallelBegin(int* argc, char*** argv);
void parallelEnd();

CellMessage* createCellMessage(Parameters* parameters);
void freeCellMessage(CellMessage* message);
int isRootRank();

void synchronize();
int hasMessages();
void fillSendCellsArray(CellMessage* send_cells, int send_count, CellMap* map, int* block, Parameters* parameters);
void receiveCellArrayAtBlock(CellMessage* array, int* count, int* block, Parameters* parameters);
void dumpCellArrayToBlock(CellMap* map, CellMessage* receive_cells, int receive_count);

void sendBlock(CellMessage* send_cells, int send_count, CellMap* map, int* block, Parameters* parameters);
void receiveBlock(CellMap* map, CellMessage* receive_cells, Parameters* parameters);

void getLocalBlockRange(int* blocks, Parameters* parameters);

Model* gatherModel(Model* model);
Model* scatterModel(Model* model);
#endif /* PARALLEL_H */
