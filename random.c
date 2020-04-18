#include <stdlib.h>
#include "utils.h"

void randomSeed(unsigned long seed){
	srand(seed);
}

unsigned int getMaxRandomValue(){
	return RAND_MAX;
}

unsigned int randomValue(){
	return rand();
}

unsigned int randomInt(unsigned int max){
	return randomValue() % max;
}

int randomUniformEvent(double probability){
	double threshold = min(probability, 1) * RAND_MAX;
	double event = rand();
	return event < threshold;
}
