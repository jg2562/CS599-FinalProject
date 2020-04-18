#ifndef CELL_H
#define CELL_H

#include "parameters.h"
#include "condition.h"

typedef enum Cell{unknown,empty,infected,susceptible,dead,immune,mountain,store} Cell;

Cell charToCell(char c);
char cellToChar(Cell cell);

Cell createCell();
Cell randomCell();

int cellHasEffect(Cell* cell);
double getEffectRadius(Cell* cell, Parameters* parameters);
void applyCellEffect(Cell* cell, Cell* target, Parameters* parameters, Condition* target_condition);

void applyConditionsToCell(Cell* cell, Parameters* parameters, Condition* condition, unsigned int time_step);

int isSusceptible(Cell* cell);
int isInfected(Cell* cell);
int isImmune(Cell* cell);
int isDead(Cell* cell);
int isCreature(Cell* cell);

#endif /* CELL_H */
