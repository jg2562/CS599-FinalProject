#ifndef CELL_H
#define CELL_H

#include "parameters.h"
#include "condition.h"

typedef enum Cell{unknown,empty,infected,susceptible,dead,immune,mountain,store} Cell;

Cell charToCell(char c);
char cellToChar(Cell cell);

Cell createCell();

int cellHasEffect(Cell* cell);
double getEffectRadius(Cell* cell, Parameters* parameters);
void applyCellEffect(Cell* cell, Cell* target, Parameters* parameters, Condition* target_condition);

void applyConditionsToCell(Cell* cell, Parameters* parameters, Condition* condition, unsigned int time_step);

int isInfected(Cell* cell);
int isCreature(Cell* cell);

#endif /* CELL_H */
