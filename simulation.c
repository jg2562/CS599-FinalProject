#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "model.h"

void testSimulation(const char* parameter_file, const char* map_file);

int main(int argc, char** argv){
	if (argc != 3){
		fprintf(stderr, "===========================\n");
		fprintf(stderr, "Error: 2 parameters expected, %d were given.\n", argc);
		fprintf(stderr, "Proper Usage:\n");
		fprintf(stderr, "\t%s <parameter file> <map file>\n", argv[0]);
		fprintf(stderr, "===========================\n");
	}
	testSimulation(argv[1], argv[2]);
	exit(0);
}

void testSimulation(const char* parameter_file, const char* map_file){

	Model* model = importModel(parameter_file, map_file);
	if (model == NULL){
		fprintf(stderr, "Failed to import model.\n");
		exit(1);
	}

	int width = model->parameters->model_width;
	int height = model->parameters->model_height;

	printCellMap(model->map, width, height);
	freeModel(model);

}


