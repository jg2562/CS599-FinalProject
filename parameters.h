#ifndef PARAMETERS_H
#define PARAMETERS_H
#include "linkedList.h"

typedef struct Parameters {
	double spread_rate;
	double spread_radius;
	double recovery_rate;
	int model_height;
	int model_width;
	int block_height;
	int block_width;
	int seed;
	int simulation_iterations;

} Parameters;

typedef struct ParseParameter{
	char* name;
	char* value;
} ParseParameter;

Parameters* createParameters();
Parameters* parseParameters(LinkedList* parameter_list);
void freeParameters(Parameters* parameters);
void printParameters(Parameters* parameters);

ParseParameter* createParseParameter(const char* name, const char* value);
void freeParseParameter(void* parameter);
void freeParseList(LinkedList* list);

#endif /* PARAMETERS_H */
