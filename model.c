#include "model.h"

#include <stdlib.h>
#include <stdio.h>

#include "population.h"

Model* createModel(){
	Model* model = malloc(sizeof(*model));
	model->map = NULL;
	model->parameters = NULL;
	model->population = createPopulation();
	return model;
}


void freeModel(Model* model){
	if (model == NULL){
		return;
	}

	int height = model->parameters->model_height;
	freeCellMap(model->map, height);
	freeParameters(model->parameters);
	freePopulation(model->population);
	free(model);
}

void pollPopulation(Model* model){
	Population* population = model->population;
	Parameters* parameters = model->parameters;
	int width = parameters->model_width;
	int height = parameters->model_height;
	CellMap* map = model->map;

	initializePopulation(population);

	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			Cell* current = &map[row][col];

			pollCell(population, current);
		}
	}
}

void printModel(Model* model){
	int width = model->parameters->model_width;
	int height = model->parameters->model_height;

	printParameters(model->parameters);
	printf("\n");
	printCellMap(model->map, width, height);
	printf("\n");
	printPopulation(model->population);

}

