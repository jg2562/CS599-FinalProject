#include "simulation.h"

#include <stdlib.h>
#include <stdio.h>

#include "conditionMap.h"
#include "condition.h"
#include "utils.h"
#include "cell.h"

void stepSimulation(Model* model);
void simulateCells(Model *model, ConditionMap* conditions);
void simulateCell(Model* model, Cell* current, Condition* condition);
void runIterationFunction(Model* model, void(*iterationFunction) (Model*));

void getBounds(int min_index[2], int max_index[2], double radius, int position[2], int size[2]);
void applyEffect(Model* model, ConditionMap* condition_map, Cell* current, int min_index[2], int max_index[2]);
void spreadCellCondition(Model* model, ConditionMap* condition_map,  int position[2]);
void getCellsConditions(Model *model, ConditionMap* condition_map);

void testSimulation(const char* parameter_file, const char* map_file){

	Model* model = importModel(parameter_file, map_file);
	if (model == NULL){
		fprintf(stderr, "Failed to import model.\n");
		exit(1);
	}

	printModel(model);

	runSimulation(model);

	printModel(model);

	freeModel(model);

}

void runSimulation(Model* model){
	runSimulationIterator(model, NULL);
}

void runSimulationIterator(Model* model, void(*iterationFunction) (Model*)){
	srand(model->parameters->seed);

	int iterations = model->parameters->simulation_iterations;

	runIterationFunction(model, iterationFunction);
	for (int i = 0; i < iterations; i++){
		stepSimulation(model);
		runIterationFunction(model, iterationFunction);
	}

}

void runIterationFunction(Model* model, void(*iterationFunction) (Model*)){
	if (iterationFunction != NULL){
		iterationFunction(model);
	}
}

void stepSimulation(Model* model){

	Parameters* parameters  = model->parameters;

	int width = parameters->model_width;
	int height = parameters->model_height;

	ConditionMap* conditions = createConditionMap(width, height);

	getCellsConditions(model, conditions);

	simulateCells(model, conditions);

	freeConditionMap(conditions, height);

}

void getCellsConditions(Model *model, ConditionMap* condition_map) {
	Parameters *parameters = model->parameters;

	int width = parameters->model_width;
	int height = parameters->model_height;
	int position[2] = {-1,-1};

	for (int row = 0; row < height; row++) {
		position[1] = row;
		for (int col = 0; col < width; col++) {
			position[0] = col;

			spreadCellCondition(model, condition_map, position);
		}
	}
}

void spreadCellCondition(Model* model, ConditionMap* condition_map,  int position[2]){
	int x = position[0];
	int y = position[1];


	Parameters* parameters = model->parameters;
	int min_index[2] = {-1,-1};
	int max_index[2] = {-1,-1};
	int size[2] = {parameters->model_width,parameters->model_height};


	Cell* cell = &model->map[y][x];
	double radius = getEffectRadius(cell, parameters);

	getBounds(min_index,max_index,radius,position, size);

	applyEffect(model, condition_map, cell, min_index, max_index);

}

void getBounds(int min_index[2], int max_index[2], double radius, int position[2], int size[2]){

	min_index[0] = max(position[0] - radius, 0);
	min_index[1] = max(position[1] - radius, 0);

	max_index[0] = min(position[0] + radius+1, size[0]);
	max_index[1] = min(position[1] + radius+1, size[1]);

}

void applyEffect(Model* model, ConditionMap* condition_map, Cell* current, int min_index[2], int max_index[2]){

	Parameters* parameters = model->parameters;
	CellMap* map = model->map;

	for (int row = min_index[1]; row < max_index[1]; row++) {
		for (int col = min_index[0]; col < max_index[0]; col++) {
			Cell* target = &map[row][col];
			Condition* target_conditions = &condition_map[row][col];

			applyCellEffect(current, target, parameters, target_conditions);
		}
	}
}

void simulateCells(Model *model, ConditionMap* conditions) {

	initializePopulation(model->population);
	Parameters *parameters = model->parameters;

	int width = parameters->model_width;
	int height = parameters->model_height;

	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			Cell* current = &model->map[row][col];
			Condition* condition = &conditions[row][col];

			simulateCell(model, current, condition);
		}
	}
}

void simulateCell(Model* model, Cell* current, Condition* condition){

	applyConditionsToCell(current, condition);

	pollCell(model->population, current);
}

