#include "simulation.h"

#include <stdlib.h>
#include <stdio.h>


void stepSimulation(Model* model);
void simulateCells(Model *model, CellMap *next_map);
void simulateCell(Model* model, CellMap* next_map, int position[2]);

void testSimulation(const char* parameter_file, const char* map_file){

	Model* model = importModel(parameter_file, map_file);
	if (model == NULL){
		fprintf(stderr, "Failed to import model.\n");
		exit(1);
	}

	int width = model->parameters->model_width;
	int height = model->parameters->model_height;

	printParameters(model->parameters);
	printf("\n");
	printCellMap(model->map, width, height);
	freeModel(model);

}


void runSimulation(Model* model){

	int iterations = model->parameters->simulation_iterations;

	for (int i = 0; i < iterations; i++){
		stepSimulation(model);
	}

}

void stepSimulation(Model* model){

	Parameters* parameters  = model->parameters;

	int width = parameters->model_width;
	int height = parameters->model_height;

	CellMap* next_map = createCellMap(width, height);

	simulateCells(model, next_map);

	CellMap* previous_map = model->map;

	model->map = next_map;

	freeCellMap(previous_map, height);

}

void simulateCells(Model *model, CellMap *next_map) {
	Parameters *parameters = model->parameters;

	int width = parameters->model_width;
	int height = parameters->model_height;
	int position[2];

	for (int row = 0; row < height; row++) {
		position[1] = row;
		for (int col = 0; col < width; col++) {
			position[0] = col;

			simulateCell(model, next_map, position);
		}
	}
}

void simulateCell(Model* model, CellMap* next_map, int position[2]){
	int x = position[0];
	int y = position[1];
	printf("simulating %d,%d!\n", x, y);

}
