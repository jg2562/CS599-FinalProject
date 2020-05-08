#include "simulationData.h"

#include <stdio.h>
#include <stdlib.h>

#include "parameters.h"
#include "block.h"

void freeSendMatrix(int** matrix, Parameters* parameters);
int** createSendMatrix(Parameters* parameters);

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
	data->send_count = createSendMatrix(parameters);

	getIterationBlocks(data->block_range, parameters);

	return data;
}

int** createSendMatrix(Parameters* parameters){
	int dimensions[2];
	getMapDimensionsInBlocks(dimensions, parameters);

	int** matrix = malloc(sizeof(*matrix) * dimensions[1]);
	for (int i = 0; i < dimensions[1]; i++){
		matrix[i] = malloc(sizeof(**matrix) * dimensions[0]);
		for (int j = 0; j < dimensions[0]; j++){
			matrix[i][j] = 0;
		}
	}

	return matrix;
}

void freeSimulationData(SimulationData* data){
	Model* model = data->model;
	int height = getParameters(model)->model_height;
	freeConditionMap(data->conditions, height);
	freeSendMatrix(data->send_count, getParameters(model));
}

void freeSendMatrix(int** matrix, Parameters* parameters){
	int dimensions[2];
	getMapDimensionsInBlocks(dimensions, parameters);

	for (int i = 0; i < dimensions[1]; i++){
		free(matrix[i]);
	}
	free(matrix);
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
	iteration->send_count = data->send_count;

	return iteration;
}

void freeIteration(Iteration* iteration){
	free(iteration);
}

