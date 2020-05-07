#ifndef SIMULATIONDATA_H
#define SIMULATIONDATA_H
#include "model.h"
#include "conditionMap.h"

typedef struct SimulationData{
	Model* model;
	int block_range[2];
	ConditionMap* conditions;
} SimulationData;

typedef struct Iteration{
	unsigned int time_step;
	Model* model;
	int block_range[2];
	ConditionMap* conditions;
} Iteration;

SimulationData* createSimulationData(Model* model);
void freeSimulationData(SimulationData* data);
Iteration* createIteration(SimulationData* data, unsigned int time_step);
void freeIteration(Iteration* iteration);
int segmentDimension(int length, int segmentation);

#endif /* SIMULATIONDATA_H */
