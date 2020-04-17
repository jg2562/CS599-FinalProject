#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "simulation.h"

Model* loadModel(const char* parameter_file, const char* map_file);
void testSimulation(const char* parameter_file, const char* map_file);
void animateSimulation(const char* parameter_file, const char* map_file);
void clearAndPrintModel(Model* model);

int main(int argc, char** argv){
	if (argc != 4){
		fprintf(stderr, "===========================\n");
		fprintf(stderr, "Error: 3 parameters expected, %d were given.\n", argc);
		fprintf(stderr, "Proper Usage:\n");
		fprintf(stderr, "\t%s <operation mode> <parameter file> <map file>\n", argv[0]);
		fprintf(stderr, "===========================\n");
	}

	const char *decision = argv[1];

	if (strcmp(decision, "test") == 0) {
		testSimulation(argv[2], argv[3]);
	} else if (strcmp(decision, "animate") == 0) {
		animateSimulation(argv[2], argv[3]);
	} else {
		// Report an invalid flag
		fprintf(stderr, "Invalid operation mode: %s\n", argv[1]);
		exit(-1);
	}

	exit(0);
}


void testSimulation(const char* parameter_file, const char* map_file){

	Model* model = loadModel(parameter_file, map_file);

	printModel(model);

	runSimulation(model);

	printModel(model);

	freeModel(model);

}

void animateSimulation(const char* parameter_file, const char* map_file){
	Model* model = loadModel(parameter_file, map_file);

	runSimulationIterator(model, clearAndPrintModel);
}

void clearAndPrintModel(Model* model){
	for(int i = 0; i < 100; i++){
		printf("\n");
	}

	printModel(model);

	usleep(100000);
}

Model* loadModel(const char* parameter_file, const char* map_file){
	Model* model = importModel(parameter_file, map_file);

	if (model == NULL){
		fprintf(stderr, "Failed to import model.\n");
		exit(1);
	}

	return model;
}
