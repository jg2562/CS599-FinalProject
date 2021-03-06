#ifndef MODEL_H
#define MODEL_H

#include "parameters.h"
#include "cellMap.h"
#include "population.h"

// Holds all data related to the simulation model
typedef struct Model{
	CellMap* map;
	Parameters* parameters;
	Population* population;
} Model;

Model* createModel();
Model* createFilledModel(Parameters* parameters, CellMap* cell_map);
void freeModel(Model* model);

void pollPopulation(Model* model);

Parameters* getParameters(Model* model);
Population* getPopulation(Model* model);
CellMap* getCellMap(Model* model);

void printModel(Model* model);
#endif /* MODEL_H */
