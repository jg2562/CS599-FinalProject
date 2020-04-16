#ifndef CELLMAP_H
#define CELLMAP_H

#include "cell.h"

// Only one deference to match calling conventions

// Holds a 2d array of cells
typedef Cell* CellMap;


CellMap* createCellMap(int width, int height);
void freeCellMap(CellMap* map, int height);

void printCellMap(CellMap* map, int width, int height);

#endif /* CELLMAP_H */
