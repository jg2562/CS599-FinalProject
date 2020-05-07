#ifndef SIMULATIONDATA_H
#define SIMULATIONDATA_H
#include "model.h"
#include "conditionMap.h"

typedef struct Iteration{
	unsigned int time_step;
	Model* model;
	ConditionMap* conditions;
	int block_range[2];
} Iteration;

Iteration* createIteration(Model* model, unsigned int time_step, int* block_range);
void freeIteration(Iteration* iteration);

#endif /* SIMULATIONDATA_H */
