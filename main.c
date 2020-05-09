#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "randomMap.h"
#include "simulation.h"
#include "parser.h"
#include "model.h"

void testSimulation(const char* parameter_file, const char* map_file);
void animateSimulation(const char* parameter_file, const char* map_file);
void timeSimulation(const char* parameter_file, const char* map_file);

Model* loadModel(const char* parameter_file, const char* map_file);
void runFullSimulation(const char* parameter_file, const char* map_file);
void clearAndPrintModel(Model* model);
Model* generateModel(const char* parameter_file);
void usleep(unsigned int);

int main(int argc, char** argv){
	if (3 > argc || argc > 4){
		fprintf(stderr, "===========================\n");
		fprintf(stderr, "Error: 3 parameters expected, %d were given.\n", argc);
		fprintf(stderr, "Proper Usage:\n");
		fprintf(stderr, "\t%s <operation mode> <parameter file> [map file]\n", argv[0]);
		fprintf(stderr, "===========================\n");
		exit(1);
	}

	const char* decision = argv[1];
	const char* parameter_file = argv[2];
	const char* map_file = argv[3];

	if (strcmp(decision, "test") == 0) {
		testSimulation(parameter_file, map_file);
	} else if (strcmp(decision, "run") == 0) {
		runFullSimulation(parameter_file, map_file);
	} else if (strcmp(decision, "animate") == 0) {
		animateSimulation(parameter_file, map_file);
	} else if (strcmp(decision, "time") == 0) {
		timeSimulation(parameter_file, map_file);
	} else {
		// Report an invalid flag
		fprintf(stderr, "Invalid operation mode: %s.\n\n", argv[1]);
		fprintf(stderr, "Valid operation modes: run, time, animate, test.\n");
		exit(-1);
	}

	exit(0);
}

void runFullSimulation(const char* parameter_file, const char* map_file){
	Model* model = loadModel(parameter_file, map_file);
	runSimulation(model);
	freeModel(model);
}

void testSimulation(const char* parameter_file, const char* map_file){

	Model* model = loadModel(parameter_file, map_file);

	printModel(model);
	printf("\n==============\n\n");

	runSimulation(model);

	printModel(model);

	freeModel(model);

}

void animateSimulation(const char* parameter_file, const char* map_file){
	Model* model = loadModel(parameter_file, map_file);

	runSimulationIterator(model, clearAndPrintModel);
}

void timeSimulation(const char* parameter_file, const char* map_file){
	Model* model = loadModel(parameter_file, map_file);
	time_t simulation_start_time = time(NULL);
	runSimulation(model);
	time_t simulation_end_time = time(NULL);
	double seconds = difftime(simulation_end_time, simulation_start_time);
	printf("Simulation time: %.1lfs\n", seconds);
}

void clearAndPrintModel(Model* model){
	unsigned int usecs = 100000;

	for(int i = 0; i < 100; i++){
		printf("\n");
	}

	printModel(model);

	usleep(usecs);
}

Model* loadModel(const char* parameter_file, const char* map_file){
	Model* model;
	if (map_file == NULL){
		model = generateModel(parameter_file);
	} else {
		model = importModel(parameter_file, map_file);
	}

	if (model == NULL){
		fprintf(stderr, "Failed to import model.\n");
		exit(1);
	}

	pollPopulation(model);

	return model;
}


Model* generateModel(const char* parameter_file){

	Parameters* parameters = importParameters(parameter_file);
	if (parameters == NULL){
		fprintf(stderr, "Failed to import parameters.\n");
		return NULL;
	}

	CellMap* map = generateRandomMap(parameters);

	Model* model = createFilledModel(parameters, map);
	return model;
}
