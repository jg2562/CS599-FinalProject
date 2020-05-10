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

int validParallelParameters(Parameters* parameters);
void synchronize();
int hasMessages();
void fillSendCellsArray(CellMessage* send_cells, int send_count, CellMap* map, int* block, Parameters* parameters);
void receiveCellArrayAtBlock(CellMessage* array, int* count, int* block, Parameters* parameters);
void dumpCellArrayToBlock(CellMap* map, CellMessage* receive_cells, int* block, int receive_count, Parameters* parameters);

void sendBlock(CellMessage* send_cells, int send_count, CellMap* map, int* block, Parameters* parameters);
void receiveBlock(CellMap* map, CellMessage* receive_cells, Parameters* parameters);

void getLocalBlockRange(int* blocks, Parameters* parameters);

void gatherModel(Model* model);
void scatterModel(Model* model);
Model* scatterAndInitializeModel(Model* model);
#endif /* PARALLEL_H */
