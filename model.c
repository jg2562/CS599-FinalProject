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

	int height = getParameters(model)->model_height;
	freeCellMap(getCellMap(model), height);
	freeParameters(getParameters(model));
	freePopulation(getPopulation(model));
	free(model);
}

void pollPopulation(Model* model){
	Population* population = getPopulation(model);
	Parameters* parameters = getParameters(model);
	int width = parameters->model_width;
	int height = parameters->model_height;
	CellMap* map = getCellMap(model);

	initializePopulation(population);

	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			Cell* current = &map[row][col];

			pollCell(population, current);
		}
	}
}

Population* getPopulation(Model* model){
	return model->population;
}

Parameters* getParameters(Model* model){
	return model->parameters;
}

CellMap* getCellMap(Model* model){
	return model->map;
}

void printModel(Model* model){
	int width = getParameters(model)->model_width;
	int height = getParameters(model)->model_height;

	printParameters(getParameters(model));
	printf("\n");
	printCellMap(getCellMap(model), width, height);
	printf("\n");
	printPopulation(getPopulation(model));

}

