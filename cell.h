#ifndef CELL_H
#define CELL_H

typedef enum Cell{unknown,empty,infected,susceptible,dead,immune,mountain,store} Cell;

Cell charToCell(char c);
char cellToChar(Cell cell);

#endif /* CELL_H */
