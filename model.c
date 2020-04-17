#include "model.h"

#include <stdlib.h>
#include <stdio.h>

void freeMap(Model* model);

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

	freeMap(model);
	freeParameters(model->parameters);
	freePopulation(model->population);
	free(model);
}

void freeMap(Model* model){
	if(model->map == NULL){
		return;
	}

	Cell** map = model->map;
	int height = model->parameters->model_height;
	for (int i = 0; i < height; i++){
		free(map[i]);
	}
	free(map);
	model->map = NULL;
}
