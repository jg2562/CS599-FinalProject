#include "parser.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "cellMap.h"

//function prototypes

int parseParameter(Parameters* parameter, const char* parameter_name, const char* parameter_value);
int extractParameterData(char** parameter_name, char** parameter_value, char* parameter_line);
int parseParameterLine(Parameters* parameter,char* parameter_line);
int parseInt(const char* str_value);
double parseDouble(const char* str_value);

int readCellMap(CellMap* map, FILE* fp, int width, int height);
int readCellLine(Cell* cells, FILE* fp, int width);
int parseCellLine(Cell* cells, char* buffer, int width);

Model* importModel(const char* parameter_file, const char* map_file){
	Parameters* parameters = importParameters(parameter_file);
	if (parameters == NULL){
		fprintf(stderr, "Failed to import parameters.\n");
		return NULL;
	}

	CellMap* map = importCellMap(map_file, parameters->model_width, parameters->model_height);
	if (map == NULL){
		fprintf(stderr, "Failed to import cell map.\n");
		freeParameters(parameters);
		return NULL;
	}

	Model* model = createModel();
	model->parameters = parameters;
	model->map = map;
	return model;
}

Parameters* importParameters(const char* fname){
	const unsigned buffer_size = 4096;

	FILE *fp = fopen(fname, "r");

	if (!fp) {
		fprintf(stderr,"Unable to open file\n");
		return NULL;
	}

	char buffer[buffer_size];

	Parameters* parameters = createParameters();

	int parsed = 1;
	while (fgets(buffer, buffer_size, fp) && parsed) {
		parsed = parseParameterLine(parameters, buffer);
	}
	parsed &= checkParameters(parameters);

	if (parsed){
		return parameters;
	} else{
		freeParameters(parameters);
		return NULL;
	}
}

 int parseParameterLine(Parameters* parameters, char* parameter_line){
	char* parameter_name;
	char* parameter_value;
	if(extractParameterData(&parameter_name, &parameter_value, parameter_line)){
		return parseParameter(parameters,parameter_name, parameter_value);
	}
	return 0;
}

int extractParameterData(char** parameter_name, char** parameter_value, char* parameter_line){
	*parameter_name = strtok(parameter_line, ":");
	*parameter_value = strtok(NULL, ":");

	if (*parameter_value == NULL){
		return 0;
	}
	return 1;
}

int parseParameter(Parameters* parameter, const char* parameter_name, const char* parameter_value){
	if (strcmp("Spread rate",parameter_name) == 0){
		parameter->spread_rate = parseDouble(parameter_value);

	} else if(strcmp("Recovery rate", parameter_name) == 0){
		parameter->recovery_rate = parseDouble(parameter_value);

	} else if(strcmp("Model height", parameter_name) == 0){
		parameter->model_height = parseInt(parameter_value);

	} else if(strcmp("Model width", parameter_name) == 0){
		parameter->model_width = parseInt(parameter_value);

	} else if(strcmp("Seed", parameter_name) == 0){
		parameter->seed = parseInt(parameter_value);
	} else {
		fprintf(stderr,"Unrecognized parameter: %s\n", parameter_name);
		return 0;
	}
	return 1;
}

double parseDouble(const char* str_value){
	double val;
	sscanf(str_value, "%lf", &val);
	return val;
}

int parseInt(const char* str_value){
	int val;
	sscanf(str_value, "%d", &val);
	return val;
}



CellMap* importCellMap(const char* map_file, int width, int height){

	FILE *fp = fopen(map_file, "r");

	if (!fp) {
		fprintf(stderr,"Unable to open file.\n");
		return NULL;
	}

	CellMap* map = createCellMap(width, height);

	int parsed = readCellMap(map, fp, width, height);

	if (parsed){
		return map;
	} else {
		freeCellMap(map, height);
		return NULL;
	}
}

int readCellMap(CellMap* map, FILE* fp, int width, int height){
	int parsed = 1;
	int i;
	for (i = 0; i < height && parsed; i++){
		parsed = readCellLine(map[i], fp, width);
	}
	if (!parsed){
		fprintf(stderr,"Bad line read at %d\n", i+1);
	}
	return parsed;
}

int readCellLine(Cell* cells, FILE* fp, int width){
	const unsigned buffer_size = 4096;

	char buffer[buffer_size];

	if(fgets(buffer, buffer_size,fp)){
		int parsed = parseCellLine(cells, buffer, width);

		return parsed;
	}
	return 0;
}

int parseCellLine(Cell* cells, char* buffer, int width){
	if (buffer[width] != '\0' && buffer[width] != '\n'){
		return 0;
	}

	for (int i = 0; i < width; i++){
		if (buffer[i] == '\0'){
			return 0;
		}

		cells[i] = charToCell(buffer[i]);
	}

	return 1;
}
