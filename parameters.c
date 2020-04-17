#include <stdio.h>
#include <stdlib.h>
#include "parameters.h"


Parameters* createParameters(){
	Parameters* parameters = malloc(sizeof(*parameters));
	parameters->spread_rate = -1;
	parameters->spread_radius = -1;
	parameters->recovery_rate = -1;
	parameters->model_height = -1;
	parameters->model_width = -1;
	parameters->seed = -1;
	parameters->simulation_iterations = -1;
	return parameters;
}

int checkParameters(Parameters* parameters){
	int valid = 1;
	if (parameters->spread_rate < 0){
		fprintf(stderr,"Parameter spread rate invalid value: %lf\n", parameters->spread_rate);
		valid = 0;
	}

	if (parameters->spread_radius < 0){
		fprintf(stderr,"Parameter spread radius invalid value: %lf\n", parameters->spread_radius);
		valid = 0;
	}

	if (parameters->recovery_rate < 0){
		fprintf(stderr,"Parameter recovery rate invalid value: %lf\n", parameters->recovery_rate);
		valid = 0;
	}

	if (parameters->model_height <= 0){
		fprintf(stderr,"Parameter model height invalid value: %d\n", parameters->model_height);
		valid = 0;
	}

	if (parameters->model_width <= 0){
		fprintf(stderr,"Parameter model width invalid value: %d\n", parameters->model_width);
		valid = 0;
	}

	if (parameters->seed < 0){
		fprintf(stderr,"Parameter seed invalid value: %d\n", parameters->seed);
		valid = 0;
	}

	if (parameters->simulation_iterations < 0){
		fprintf(stderr,"Parameter simulation iterations invalid value: %d\n", parameters->simulation_iterations);
		valid = 0;
	}

	return valid;
}

void freeParameters(Parameters* parameters){
	free(parameters);
}

void printParameters(Parameters* parameters){
	printf("Spread rate: %lf\n", parameters->spread_rate);
	printf("Spread radius: %lf\n", parameters->spread_radius);
	printf("Recovery rate: %lf\n", parameters->recovery_rate);
	printf("Model height: %d\n", parameters->model_height);
	printf("Model width: %d\n", parameters->model_width);
	printf("Seed: %d\n", parameters->seed);
	printf("Simulation iterations: %d\n", parameters->simulation_iterations);
}
