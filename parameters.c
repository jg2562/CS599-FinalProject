#include <stdio.h>
#include <stdlib.h>
#include "parameters.h"


Parameters* createParameters(){
	Parameters* params = malloc(sizeof(*params));
	params->spread_rate = -1;
	params->recovery_rate = -1;
	params->model_height = -1;
	params->model_width = -1;
	params->seed = -1;
	return params;
}

int checkParameters(Parameters* parameters){
	int valid = 1;
	if (parameters->spread_rate < 0){
		fprintf(stderr,"Parameter spread rate invalid value: %lf\n", parameters->spread_rate);
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
		fprintf(stderr,"Parameter model height invalid value: %d\n", parameters->model_width);
		valid = 0;
	}

	if (parameters->seed < 0){
		fprintf(stderr,"Parameter seed invalid value: %d\n", parameters->seed);
		valid = 0;
	}


	return valid;
}

void freeParameters(Parameters* parameters){
	free(parameters);
}
