#include "simulation.h"

#include <stdlib.h>
#include <stdio.h>

#include "conditionMap.h"
#include "condition.h"
#include "utils.h"
#include "cell.h"
#include "random.h"
#include "simulationData.h"
#include "block.h"

void stepSimulation(SimulationData* data, unsigned int time_step);
void simulateCells(Iteration* iteration);
void simulateBlock(Iteration* iteration, int* block);
void simulateCell(Model* model, Cell* current, Condition* condition, unsigned int time_step);
void runIterationFunction(Model* model, void(*iterationFunction) (Model*));

void getBounds(int min_index[2], int max_index[2], double radius, int position[2], int size[2]);
void applyEffect(Iteration* iteration, Cell* current, int min_index[2], int max_index[2]);
void spreadCellCondition(Iteration* iteration,  int position[2]);
void getCellsConditions(Iteration* iteration);
void getCellBlockConditions(Iteration* iteration, int* block);

void runSimulation(Model* model){
	runSimulationIterator(model, NULL);
}

void runSimulationIterator(Model* model, void(*iterationFunction) (Model*)){
	randomSeed(getParameters(model)->seed);

	Parameters* parameters = getParameters(model);
	int iterations = parameters->simulation_iterations;
	SimulationData* data = createSimulationData(model);

	runIterationFunction(model, iterationFunction);
	for (int i = 0; i < iterations; i++){
		stepSimulation(data, i);
		runIterationFunction(model, iterationFunction);
	}

}

void runIterationFunction(Model* model, void(*iterationFunction) (Model*)){
	if (iterationFunction != NULL){
		iterationFunction(model);
	}
}

void stepSimulation(SimulationData* data, unsigned int time_step){

	Iteration* iteration = createIteration(data, time_step);

	getCellsConditions(iteration);

	simulateCells(iteration);

	freeIteration(iteration);
}

void getCellsConditions(Iteration* iteration) {
	Parameters *parameters = getParameters(iteration->model);

	int block[2] = {-1,-1};
	int* block_range = iteration->block_range;

	for (int block_index = block_range[0]; block_index < block_range[1]; block_index++){
		blockIndexToPosition(block, block_index, parameters);
		getCellBlockConditions(iteration, block);
	}
}

void getCellBlockConditions(Iteration* iteration, int* block){
	Parameters *parameters = getParameters(iteration->model);

	int position[2] = {-1,-1};
	int block_dimensions[2] = {-1,-1};
	int block_origin[2] = {-1,-1};

	getBlockOrigin(block_origin, block, parameters);
	getBlockDimensions(block_dimensions, block, parameters);

	for (int row = 0; row < block_dimensions[1]; row++) {
		position[1] = block_origin[1] + row;
		for (int col = 0; col < block_dimensions[0]; col++) {
			position[0] = block_origin[0] + col;

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


	int block[2] = {-1,-1};
	int* block_range = iteration->block_range;

	for (int block_index = block_range[0]; block_index < block_range[1]; block_index++){
		blockIndexToPosition(block, block_index, parameters);
		simulateBlock(iteration, block);
	}
}

void simulateBlock(Iteration* iteration, int* block){

	Model* model = iteration->model;
	ConditionMap* conditions = iteration->conditions;

	Parameters *parameters = getParameters(model);
	unsigned int time_step = iteration->time_step;

	int block_dimensions[2] = {-1,-1};
	int block_origin[2] = {-1,-1};

	getBlockOrigin(block_origin, block, parameters);
	getBlockDimensions(block_dimensions, block, parameters);

	CellMap* map = getCellMap(model);

	int row;
	int col;
	for (int sub_row = 0; sub_row < block_dimensions[1]; sub_row++) {
		row = block_origin[1] + sub_row;
		for (int sub_col = 0; sub_col < block_dimensions[0]; sub_col++) {
			col = block_origin[0] + sub_col;

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
