#include "model.h"

#include <stdlib.h>
#include <stdio.h>

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
