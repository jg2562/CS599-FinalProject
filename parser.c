#include "parser.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "utils.h"
#include "cellMap.h"
#include "model.h"
#include "linkedList.h"
#include "parallel.h"

//function prototypes

LinkedList* readParameters(const char* fname);
int parseParameter(Parameters* parameter, const char* parameter_name, const char* parameter_value);
int extractParameterData(char** parameter_name, char** parameter_value, char* parameter_line);
int readParameterLine(LinkedList** parsed_list, char* parameter_line);
void addParameterToList(LinkedList** parsed_list, const char* name, const char* value);
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
	} else if(!validParallelParameters(parameters)){
		fprintf(stderr, "Invalid model parameters for specified parallel parameters.\n");
		freeParameters(parameters);
		return NULL;
	}

	CellMap* map = importCellMap(map_file, parameters->model_width, parameters->model_height);
	if (map == NULL){
		fprintf(stderr, "Failed to import cell map.\n");
		freeParameters(parameters);
		return NULL;
	}


	Model* model = createFilledModel(parameters, map);
	return model;
}

Parameters* importParameters(const char* fname){
	LinkedList* unparsed = readParameters(fname);

	if (!unparsed){
		return NULL;
	}

	Parameters* parameters = parseParameters(unparsed);

	freeParseList(unparsed);
	return parameters;
}

LinkedList* readParameters(const char* fname){
	const unsigned buffer_size = 4096;
	FILE *fp = fopen(fname, "r");

	if (!fp) {
		fprintf(stderr,"Unable to open file\n");
		return NULL;
	}

	char buffer[buffer_size];

	LinkedList* parse_list = NULL;
	int read = 1;
	unsigned int i=0;
	while (fgets(buffer, buffer_size, fp) && read) {
		read = readParameterLine(&parse_list, buffer);
		i++;
	}

	fclose(fp);

	if (!read){
		fprintf(stderr,"Failed to read parameter line: %u.\n", i);
		freeParseList(parse_list);
		return NULL;
	}

	return parse_list;
}

int readParameterLine(LinkedList** parsed_list, char* parameter_line){
	char* parameter_name;
	char* parameter_value;

	int extracted = extractParameterData(&parameter_name, &parameter_value, parameter_line);

	if (extracted){
		addParameterToList(parsed_list,parameter_name, parameter_value);
	}
	return extracted;
}

void addParameterToList(LinkedList** parsed_list, const char* name, const char* value){
	ParseParameter* parse = createParseParameter(name, value);
	*parsed_list = appendToList(*parsed_list, parse);
}

int extractParameterData(char** parameter_name, char** parameter_value, char* parameter_line){
	*parameter_name = strtok(parameter_line, ":");
	*parameter_value = strtok(NULL, ":");
	lowerCaseString(*parameter_name);
	lowerCaseString(*parameter_value);

	if (*parameter_value == NULL){
		return 0;
	}
	return 1;
}

CellMap* importCellMap(const char* map_file, int width, int height){

	FILE *fp = fopen(map_file, "r");

	if (!fp) {
		fprintf(stderr,"Unable to open file.\n");
		return NULL;
	}

	CellMap* map = createCellMap(width, height);

	int parsed = readCellMap(map, fp, width, height);

	fclose(fp);
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
