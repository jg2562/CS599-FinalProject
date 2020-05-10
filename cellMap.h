#ifndef CELLMAP_H
#define CELLMAP_H

#include "cell.h"

// Only one deference to match calling conventions

// Holds a 2d array of cells
typedef struct CellPosition{
	int x;
	int y;
}CellPosition;

union CellEntry{
	CellPosition position;
	Cell cell;
};

typedef union CellEntry* CellMap;
typedef union CellEntry CellMapLine;



Cell* getCell(CellMap* map, int x, int y);
CellPosition* getCellPosition(CellMap* map, int x, int y);

CellMapLine* getCellMapLine(CellMap* map, int index);
Cell* getCellFromLine(CellMapLine* map, int index);

CellMap* createCellMap(int width, int height);
void freeCellMap(CellMap* map, int height);

void printCellMap(CellMap* map, int width, int height);

#endif /* CELLMAP_H */
