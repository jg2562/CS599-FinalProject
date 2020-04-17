#ifndef CELL_H
#define CELL_H

#include "parameters.h"

typedef enum Cell{unknown,empty,infected,susceptible,dead,immune,mountain,store} Cell;

Cell charToCell(char c);
char cellToChar(Cell cell);

double getEffectRadius(Cell* cell, Parameters* parameters);

#endif /* CELL_H */
