#include "simulationData.h"

#include <stdio.h>
#include <stdlib.h>


Iteration* createIteration(Model* model, unsigned int time_step, int* block_range){

	if (model == NULL){
		return NULL;
	}

	Iteration* iteration = malloc(sizeof(*iteration));

	Parameters* parameters = getParameters(model);

	int width = parameters->model_width;
	int height = parameters->model_height;

	iteration->model = model;
	iteration->time_step = time_step;
	iteration->conditions = createConditionMap(width, height);
	iteration->block_range[0] = block_range[0];
	iteration->block_range[1] = block_range[1];

	return iteration;
}

void freeIteration(Iteration* iteration){
	Model* model = iteration->model;
	int height = getParameters(model)->model_height;

	freeConditionMap(iteration->conditions, height);
	free(iteration);
}
