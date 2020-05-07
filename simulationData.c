#include "simulationData.h"

#include <stdio.h>
#include <stdlib.h>

#include "parameters.h"

void getIterationBlocks(int* blocks, Parameters* parameters);

SimulationData* createSimulationData(Model* model){
	if (model == NULL){
		return NULL;
	}

	SimulationData* data = malloc(sizeof(*data));

	Parameters* parameters = getParameters(model);

	int width = parameters->model_width;
	int height = parameters->model_height;

	data->model = model;
	data->conditions = createConditionMap(width, height);

	getIterationBlocks(data->block_range, parameters);

	return data;
}

void freeSimulationData(SimulationData* data){
	Model* model = data->model;
	int height = getParameters(model)->model_height;
	freeConditionMap(data->conditions, height);
}

Iteration* createIteration(SimulationData* data, unsigned int time_step){

	if (data == NULL){
		return NULL;
	}

	Iteration* iteration = malloc(sizeof(*iteration));

	Parameters* parameters = getParameters(data->model);

	int width = parameters->model_width;
	int height = parameters->model_height;

	iteration->model = data->model;
	iteration->time_step = time_step;
	iteration->conditions = createConditionMap(width, height);
	iteration->block_range[0] = data->block_range[0];
	iteration->block_range[1] = data->block_range[1];

	return iteration;
}

void freeIteration(Iteration* iteration){
	Model* model = iteration->model;
	int height = getParameters(model)->model_height;

	free(iteration);
}

void getIterationBlocks(int* blocks, Parameters* parameters){
	int width = segmentDimension(parameters->model_width, parameters->block_width);
	int height = segmentDimension(parameters->model_height, parameters->block_height);
	int block_count = width*height;
	blocks[0] = 0;
	blocks[1] = block_count;
}

int segmentDimension(int length, int segmentation){
	int divided = length / segmentation;
	divided += (length % segmentation) != 0;
	return divided;
}
