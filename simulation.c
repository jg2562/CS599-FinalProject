#include "simulation.h"

#include <stdlib.h>
#include <stdio.h>


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
