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
#include "parallel.h"

void stepSimulation(SimulationData* data, unsigned int time_step);
void simulateCells(Iteration* iteration);
void simulateBlock(Iteration* iteration, int* block);
void simulateCell(Model* model, Cell* current, Condition* condition, unsigned int time_step);
void runIterationFunction(Model* model, void(*iterationFunction) (Model*));

void getEffectBox(int* min_index,int* max_index, Cell* cell, int* position, Parameters* parameters);
void getBounds(int min_index[2], int max_index[2], double radius, int position[2], int size[2]);
void getBlockBounds(int min_index[2], int max_index[2], int* block, Parameters* parameters);
void applyEffectLocally(Iteration* iteration, Cell* current, int* block, int min_index[2], int max_index[2]);
void applyEffectGlobally(Iteration* iteration, Cell* current, int min_index[2], int max_index[2]);
void spreadCellCondition(Iteration* iteration, int position[2]);
void fillSendConditionArray(CellMessage* send_cells, int send_count, CellMap* map, int* block, Parameters* parameters);
void getCellsConditions(Iteration* iteration);
void getCellBlockConditions(Iteration* iteration, int* block);
void receiveAnyCellConditions(Iteration* iteration);
void sendCellsConditions(Iteration* iteration, int* block);
void applyCellMessageEffects(Iteration* iteration, CellMessage* message, int count, int* block);
void queueCellForBlock(Iteration* iteration, Cell* cell, int* block);

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
	freeSimulationData(data);
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
	synchronize();
	receiveAnyCellConditions(iteration);
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
	sendCellsConditions(iteration, block);
	receiveAnyCellConditions(iteration);
}

void spreadCellCondition(Iteration* iteration, int position[2]){
	int x = position[0];
	int y = position[1];


	Parameters* parameters = getParameters(iteration->model);
	int min_index[2] = {-1,-1};
	int max_index[2] = {-1,-1};

	CellMap* map = getCellMap(iteration->model);
	Cell* cell = getCell(map, x, y);

	if (!cellHasEffect(cell)){
		return;
	}

	getEffectBox(min_index, max_index, cell, position, parameters);

	applyEffectGlobally(iteration, cell, min_index, max_index);
}

void getEffectBox(int* min_index,int* max_index, Cell* cell, int* position, Parameters* parameters){
	int size[2] = {parameters->model_width,parameters->model_height};
	double radius = getEffectRadius(cell, parameters);

	getBounds(min_index, max_index, radius, position, size);
}

void getBounds(int min_index[2], int max_index[2], double radius, int position[2], int size[2]){

	min_index[0] = max(position[0] - radius, 0);
	min_index[1] = max(position[1] - radius, 0);

	max_index[0] = min(position[0] + radius+1, size[0]);
	max_index[1] = min(position[1] + radius+1, size[1]);

}

void getBlockBounds(int min_index[2], int max_index[2], int* block, Parameters* parameters){
	int origin[2];
	int terminus[2];

	getBlockOrigin(origin, block, parameters);
	getBlockTerminus(terminus, block, parameters);

	boundBox(min_index,max_index,origin,terminus);
}

void applyEffectGlobally(Iteration* iteration, Cell* cell, int min_index[2], int max_index[2]){
	Parameters* parameters = getParameters(iteration->model);
	int min_block[2];
	int max_block[2];
	int max_index_adjusted[2] = {max_index[0]-1,max_index[1]-1};

	globalPositionToBlock(min_block, min_index, parameters);
	globalPositionToBlock(max_block, max_index_adjusted, parameters);

	int to_block[2];
	for (int block_row = min_block[1]; block_row <= max_block[1]; block_row++){

		for (int block_col = min_block[0]; block_col <= max_block[0]; block_col++){
			to_block[0] = block_col;
			to_block[1] = block_row;
			if (isBlockLocal(to_block, parameters)){
				applyEffectLocally(iteration, cell, to_block, min_index, max_index);
			} else {
				queueCellForBlock(iteration, cell, to_block);
			}
		}
	}
}

void applyExternalEffect(Iteration* iteration, CellMessage* current, int* block){
	Parameters* parameters = getParameters(iteration->model);
	int min_index[2];
	int max_index[2];

	int min_block[2];
	int max_block[2];

	Cell* cell = &current->cell;
	int position[2] = {current->x, current->y};

	getEffectBox(min_index, max_index, cell, position, parameters);

	globalPositionToBlock(min_block, min_index, parameters);
	globalPositionToBlock(max_block, max_index, parameters);

	applyEffectLocally(iteration, cell, block, min_index, max_index);

}

void applyEffectLocally(Iteration* iteration, Cell* current, int* block, int min_index[2], int max_index[2]){
	Model* model = iteration->model;
	ConditionMap* condition_map = iteration->conditions;

	Parameters* parameters = getParameters(model);
	CellMap* map = getCellMap(model);

	int min_block[2] = {min_index[0],min_index[1]};
	int max_block[2] = {max_index[0],max_index[1]};
	getBlockBounds(min_block, max_block, block, parameters);

	for (int row = min_block[1]; row < max_block[1]; row++) {
		for (int col = min_block[0]; col < max_block[0]; col++) {
			Cell* target = getCell(map, col, row);
			Condition* target_conditions = getCondition(condition_map, col, row);

			applyCellEffect(current, target, parameters, target_conditions);
		}
	}
}

void queueCellForBlock(Iteration* iteration, Cell* cell, int* block){
	Parameters* parameters = getParameters(iteration->model);
	CellMap* map = getCellMap(iteration->model);
	int** send_counts = iteration->send_count;

	int block_position[2];
	int local_position[2];
	int row;
	int col;
	int* send_count = &send_counts[block[1]][block[0]];

	getBlockOrigin(block_position, block, parameters);
	localIndexToPosition(local_position, *send_count, block, parameters);
	(*send_count)++;

	col = block_position[0] + local_position[0];
	row = block_position[1] + local_position[1];

	Cell* loc = getCell(map, col, row);
	*loc = *cell;
}


void sendCellsConditions(Iteration* iteration, int* block){
	Parameters* parameters = getParameters(iteration->model);
	CellMap* map = getCellMap(iteration->model);
	int** send_counts = iteration->send_count;

	int map_dims[2];
	CellMessage* send_cells = createCellMessage(parameters);
	getMapDimensionsInBlocks(map_dims, parameters);

	for (int row = 0; row < map_dims[1]; row++){
		for (int col = 0; col < map_dims[0]; col++){
			int send_count = send_counts[row][col];
			if (send_count > 0){
				sendBlock(send_cells, send_count, map, block, parameters);
				send_counts[row][col] = 0;
			}
		}
	}

	freeCellMessage(send_cells);
}

void receiveAnyCellConditions(Iteration* iteration){
	Parameters* parameters = getParameters(iteration->model);

	CellMessage* message = createCellMessage(parameters);
	int block_index = 0;
	int count = 0;
	int block_pos[2];

	while (hasMessages()){
		receiveCellArrayAtBlock(message, &count, &block_index, parameters);
		blockIndexToPosition(block_pos, block_index, parameters);
		applyCellMessageEffects(iteration, message, count, block_pos);
	}
	freeCellMessage(message);
}

void applyCellMessageEffects(Iteration* iteration, CellMessage* message, int count, int* block){

	for (int i = 1; i < count; i++){
		applyExternalEffect(iteration, &message[i], block);
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

			Cell* current = getCell(map, col, row);
			Condition* condition = getCondition(conditions, col, row);

			simulateCell(model, current, condition, time_step);
		}
	}
}

void simulateCell(Model* model, Cell* current, Condition* condition, unsigned int time_step){
	Parameters* parameters = getParameters(model);

	applyConditionsToCell(current, parameters, condition, time_step);

	pollCell(getPopulation(model), current);
}
