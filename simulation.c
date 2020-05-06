#include "simulation.h"

#include <stdlib.h>
#include <stdio.h>

#include "conditionMap.h"
#include "condition.h"
#include "utils.h"
#include "cell.h"
#include "random.h"

typedef struct Iteration{
	unsigned int time_step;
	Model* model;
	ConditionMap* conditions;
} Iteration;

void stepSimulation(Model* model, unsigned int time_step);
void simulateCells(Iteration* iteration);
void simulateBlock(Iteration* iteration, int* block);
void simulateCell(Model* model, Cell* current, Condition* condition, unsigned int time_step);
void runIterationFunction(Model* model, void(*iterationFunction) (Model*));
int segmentDimension(int length, int segmentation);

void getBounds(int min_index[2], int max_index[2], double radius, int position[2], int size[2]);
void applyEffect(Iteration* iteration, Cell* current, int min_index[2], int max_index[2]);
void spreadCellCondition(Iteration* iteration,  int position[2]);
void getCellsConditions(Iteration* iteration);
void getCellBlockConditions(Iteration* iteration, int* block);

Iteration* createIteration(Model* model, unsigned int current_step);
void freeIteration(Iteration* iteration);

void runSimulation(Model* model){
	runSimulationIterator(model, NULL);
}

void runSimulationIterator(Model* model, void(*iterationFunction) (Model*)){
	randomSeed(getParameters(model)->seed);

	int iterations = getParameters(model)->simulation_iterations;

	runIterationFunction(model, iterationFunction);
	for (int i = 0; i < iterations; i++){
		stepSimulation(model, i);
		runIterationFunction(model, iterationFunction);
	}

}

void runIterationFunction(Model* model, void(*iterationFunction) (Model*)){
	if (iterationFunction != NULL){
		iterationFunction(model);
	}
}

void stepSimulation(Model* model, unsigned int time_step){

	Iteration* iteration = createIteration(model, time_step);

	getCellsConditions(iteration);

	simulateCells(iteration);

	freeIteration(iteration);
}

void getCellsConditions(Iteration* iteration) {
	Parameters *parameters = getParameters(iteration->model);

	int width = segmentDimension(parameters->model_width, parameters->block_width);
	int height = segmentDimension(parameters->model_height, parameters->block_height);

	int block[2] = {-1,-1};

	for (int block_row = 0; block_row < height; block_row++) {
		block[1] = block_row;
		for (int block_col = 0; block_col < width; block_col++) {
			block[0] = block_col;

			getCellBlockConditions(iteration, block);
		}
	}
}

int segmentDimension(int length, int segmentation){
	int divided = length / segmentation;
	divided += (length % segmentation) != 0;
	return divided;
}

void getCellBlockConditions(Iteration* iteration, int* block){
	Parameters *parameters = getParameters(iteration->model);

	int width = parameters->block_width;
	int height = parameters->block_height;
	int position[2] = {-1,-1};

	for (int row = 0; row < height; row++) {
		position[1] = block[1] * height + row;
		for (int col = 0; col < width; col++) {
			position[0] = block[0] * width + col;

			spreadCellCondition(iteration, position);
		}
	}
}

void spreadCellCondition(Iteration* iteration,  int position[2]){
	int x = position[0];
	int y = position[1];


	Parameters* parameters = getParameters(iteration->model);
	int min_index[2] = {-1,-1};
	int max_index[2] = {-1,-1};
	int size[2] = {parameters->model_width,parameters->model_height};

	CellMap* map = getCellMap(iteration->model);
	Cell* cell = &map[y][x];

	if (!cellHasEffect(cell)){
		return;
	}

	double radius = getEffectRadius(cell, parameters);

	getBounds(min_index,max_index,radius,position, size);

	applyEffect(iteration, cell, min_index, max_index);

}

void getBounds(int min_index[2], int max_index[2], double radius, int position[2], int size[2]){

	min_index[0] = max(position[0] - radius, 0);
	min_index[1] = max(position[1] - radius, 0);

	max_index[0] = min(position[0] + radius+1, size[0]);
	max_index[1] = min(position[1] + radius+1, size[1]);

}

void applyEffect(Iteration* iteration, Cell* current, int min_index[2], int max_index[2]){
	Model* model = iteration->model;
	ConditionMap* condition_map = iteration->conditions;

	Parameters* parameters = getParameters(model);
	CellMap* map = getCellMap(model);

	for (int row = min_index[1]; row < max_index[1]; row++) {
		for (int col = min_index[0]; col < max_index[0]; col++) {
			Cell* target = &map[row][col];
			Condition* target_conditions = &condition_map[row][col];

			applyCellEffect(current, target, parameters, target_conditions);
		}
	}
}

void simulateCells(Iteration* iteration) {

	Model* model = iteration->model;
	initializePopulation(getPopulation(model));
	Parameters *parameters = getParameters(model);

	int width = segmentDimension(parameters->model_width, parameters->block_width);
	int height = segmentDimension(parameters->model_height, parameters->block_height);

	int block[2] = {-1,-1};

	for (int row = 0; row < height; row++) {
		block[1] = row;
		for (int col = 0; col < width; col++) {
			block[0] = col;
			simulateBlock(iteration, block);
		}
	}
}

void simulateBlock(Iteration* iteration, int* block){

	Model* model = iteration->model;
	ConditionMap* conditions = iteration->conditions;

	Parameters *parameters = getParameters(model);
	unsigned int time_step = iteration->time_step;

	int width = parameters->block_width;
	int height = parameters->block_height;

	CellMap* map = getCellMap(model);

	int row;
	int col;
	for (int sub_row = 0; sub_row < height; sub_row++) {
		row = block[1] * height + sub_row;
		for (int sub_col = 0; sub_col < width; sub_col++) {
			col = block[0] * width + sub_col;
			Cell* current = &map[row][col];
			Condition* condition = &conditions[row][col];

			simulateCell(model, current, condition, time_step);
		}
	}
}

void simulateCell(Model* model, Cell* current, Condition* condition, unsigned int time_step){
	Parameters* parameters = getParameters(model);

	applyConditionsToCell(current, parameters, condition, time_step);

	pollCell(getPopulation(model), current);
}

Iteration* createIteration(Model* model, unsigned int time_step){

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

	return iteration;
}

void freeIteration(Iteration* iteration){
	Model* model = iteration->model;
	int height = getParameters(model)->model_height;

	freeConditionMap(iteration->conditions, height);
	free(iteration);
}
