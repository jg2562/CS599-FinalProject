#ifndef POPULATION_H
#define POPULATION_H

#include "cell.h"

typedef struct Population{
	int susceptible;
	int infected;
	int dead;
	int immune;
} Population;

Population* createPopulation();
void freePopulation(Population* population);
void initializePopulation(Population* population);

void pollCell(Population* population, Cell* cell);

#endif /* POPULATION_H */
