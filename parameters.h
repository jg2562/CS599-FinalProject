#ifndef PARAMETERS_H
#define PARAMETERS_H

typedef struct Parameters {
	double spread_rate;
	double recovery_rate;
	int model_height;
	int model_width;
	int seed;
} Parameters;


Parameters* createParameters();
int checkParameters(Parameters* parameters);
void freeParameters(Parameters* parameters);
void printParameters(Parameters* parameters);

#endif /* PARAMETERS_H */
