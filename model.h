#ifndef MODEL_H
#define MODEL_H

#include "parameters.h"
#include "cellMap.h"

// Holds all data related to the simulation model
typedef struct Model{
	CellMap* map;
	Parameters* parameters;
} Model;

Model* createModel();
void freeModel(Model* model);
#endif /* MODEL_H */
