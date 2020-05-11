#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parameters.h"
#include "utils.h"

int parseParameter(LinkedList* parameter_list, void* value, const char* name,
				   const char* parse_format, int validate(void* value), void printParameter(const char*, void*));
ParseParameter* createEmptyParseParameter();
int compareParseParameter(void* param1, void* param2);
void formatErrorMessage(char* buffer, const char* name, const char* format_type);
void printParameterNotFound(const char* name);

void printErrorParameterDouble(const char* format, void* raw_value);
void printErrorParameterInt(const char* format, void* raw_value);

int validateDoubleGreaterOrEqualToZero(void* value);
int validateIntGreaterThanZero(void* value);
int validateIntGreaterOrEqualToZero(void* value);

Parameters* createParameters(){
	Parameters* parameters = malloc(sizeof(*parameters));

	if (parameters == NULL){
		fprintf(stderr, "Failed to allocate new parameters.\n");
		exit(1);
	}

	parameters->spread_rate = -1;
	parameters->spread_radius = -1;
	parameters->recovery_rate = -1;
	parameters->model_height = -1;
	parameters->model_width = -1;
	parameters->block_height = -1;
	parameters->block_width = -1;
	parameters->seed = -1;
	parameters->simulation_iterations = -1;
	return parameters;
}

int parseInt(const char* str_value){
	int val;
	sscanf(str_value, "%d", &val);
	return val;
}

Parameters* parseParameters(LinkedList* parameter_list){
	Parameters* parameters = createParameters();
	int valid = 1;

	valid &= parseParameter(parameter_list,&parameters->spread_rate, "spread rate", "%lf",
							validateDoubleGreaterOrEqualToZero, printErrorParameterDouble);

	valid &= parseParameter(parameter_list,&parameters->spread_radius, "spread radius", "%lf",
							validateDoubleGreaterOrEqualToZero, printErrorParameterDouble);

	valid &= parseParameter(parameter_list,&parameters->recovery_rate, "recovery rate", "%lf",
							validateDoubleGreaterOrEqualToZero, printErrorParameterDouble);

	valid &= parseParameter(parameter_list,&parameters->model_height, "model height", "%d",
							validateIntGreaterThanZero, printErrorParameterInt);

	valid &= parseParameter(parameter_list,&parameters->model_width, "model width", "%d",
							validateIntGreaterThanZero, printErrorParameterInt);

	valid &= parseParameter(parameter_list,&parameters->block_height, "block height", "%d",
							validateIntGreaterThanZero, printErrorParameterInt);

	valid &= parseParameter(parameter_list,&parameters->block_width, "block width", "%d",
							validateIntGreaterThanZero, printErrorParameterInt);

	valid &= parseParameter(parameter_list,&parameters->seed, "seed", "%d",
							validateIntGreaterOrEqualToZero, printErrorParameterInt);

	valid &= parseParameter(parameter_list,&parameters->simulation_iterations, "simulation iterations", "%d",
							validateIntGreaterOrEqualToZero, printErrorParameterInt);

	if (!valid){
		freeParameters(parameters);
		return NULL;
	}

	return parameters;
}

int parseParameter(LinkedList* parameter_list, void* value, const char* name,
				   const char* parse_format, int valid(void* value), void printParameter(const char*, void*)){

	ParseParameter* find = createEmptyParseParameter();
	find->name = (char*) name;

	ParseParameter* unparsed = findItemInList(parameter_list, find, compareParseParameter);

	find->name = NULL;
	freeParseParameter(find);

	if (unparsed == NULL){
		printParameterNotFound(name);
		return 0;
	}

	sscanf(unparsed->value, parse_format, value);

	if(!valid(value)){
		char format_str[4096];
		formatErrorMessage(format_str, name, parse_format);
		printParameter(format_str, value);
		return 0;
	}

	return 1;
}

int compareParseParameter(void* param1, void* param2){
	return strcmp(((ParseParameter*) param1)->name, ((ParseParameter*) param2)->name) == 0;
}

void printParameterNotFound(const char* name){
	printf("Parameter not found: %s.\n", name);
}

void formatErrorMessage(char* buffer, const char* name, const char* format_type){
	sprintf(buffer, "Parameter %s invalid value: %s.\n", name, format_type);
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
	printf("Block height: %d\n", parameters->block_height);
	printf("Block width: %d\n", parameters->block_width);
	printf("Seed: %d\n", parameters->seed);
	printf("Simulation iterations: %d\n", parameters->simulation_iterations);
}

ParseParameter* createEmptyParseParameter(){
	ParseParameter* parsed = malloc(sizeof(*parsed));

	if (parsed == NULL){
		fprintf(stderr, "Failed to allocate new parsed.\n");
		exit(1);
	}

	parsed->name = NULL;
	parsed->value = NULL;
	return parsed;
}

ParseParameter* createParseParameter(const char* name, const char* value){
	ParseParameter* parsed = createEmptyParseParameter();
	parsed->name = duplicateString(name);
	parsed->value = duplicateString(value);
	return parsed;
}

void freeParseParameter(void* parameter){
	ParseParameter* parsed = (ParseParameter*) parameter;
	free(parsed->name);
	free(parsed->value);
	free(parsed);
}

void freeParseList(LinkedList* list){
	freeList(list, freeParseParameter);
}

int validateDoubleGreaterOrEqualToZero(void* value){
	double val = *((double*) value);
	return val >= 0;
}

int validateIntGreaterThanZero(void* value){
	int val = *((int*) value);
	return val > 0;
}

int validateIntGreaterOrEqualToZero(void* value){
	int val = *((int*) value);
	return val >= 0;
}

void printErrorParameterDouble(const char* format, void* raw_value){
	double value = *((double*) raw_value);

	fprintf(stderr, format, value);
}

void printErrorParameterInt(const char* format, void* raw_value){
	int value = *((int*) raw_value);

	fprintf(stderr, format, value);
}
