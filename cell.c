#include "cell.h"
#include <stdlib.h>
#include <stdio.h>
#include "parameters.h"
#include "condition.h"
#include "utils.h"
#include "random.h"

#define EMPTY_CHAR ' '
#define INFECTED_CHAR 'o'
#define SUSCEPTIBLE_CHAR '.'
#define DEAD_CHAR 'X'
#define IMMUNE_CHAR 'I'
#define MOUNTAIN_CHAR 'M'
#define STORE_CHAR '@'
#define DEFAULT_CHAR '?'

void transmissionEffect(Cell* target, Parameters* parameters, Condition* target_condition);
void tryInfectCell(Cell* cell, Condition* condition);

Cell createCell(){
	return unknown;
}

Cell charToCell(const char c){
	switch (c){
	case EMPTY_CHAR:
		return empty;
	case INFECTED_CHAR:
		return infected;
	case SUSCEPTIBLE_CHAR:
		return susceptible;
	case DEAD_CHAR:
		return dead;
	case IMMUNE_CHAR:
		return immune;
	case MOUNTAIN_CHAR:
		return mountain;
	case STORE_CHAR:
		return store;
	default:
		return unknown;
	}
}

char cellToChar(Cell cell){
	switch(cell){
	case empty:
		return EMPTY_CHAR;
	case infected:
		return INFECTED_CHAR;
	case susceptible:
		return SUSCEPTIBLE_CHAR;
	case dead:
		return DEAD_CHAR;
	case immune:
		return IMMUNE_CHAR;
	case mountain:
		return MOUNTAIN_CHAR;
	case store:
		return STORE_CHAR;
	default:
		return DEFAULT_CHAR;
	}
}


double getEffectRadius(Cell* cell, Parameters* parameters){

	switch(*cell){
	case infected:
		return parameters->spread_radius;
	case empty:
	case susceptible:
	case dead:
	case immune:
	case mountain:
	case store:
	default:
		return 0;
	}
	return 0;
}

int cellHasEffect(Cell* cell){
	switch(*cell){
	case infected:
		return 1;
	default:
		return 0;
	}
	return 0;
}

void applyCellEffect(Cell* cell, Cell* target, Parameters* parameters, Condition* target_condition){
	switch(*cell){
	case infected:
		transmissionEffect(target,parameters,target_condition);
		break;
	default:
		break;
	}

}

void transmissionEffect(Cell* target, Parameters* parameters, Condition* target_condition){
	if (*target == susceptible){
		target_condition->infection_probabilty += parameters->spread_rate*0.01;
	}
}

void applyConditionsToCell(Cell* cell, Condition* condition){
	switch(*cell){
	case susceptible:
		tryInfectCell(cell, condition);
		break;
	default:
		break;
	}
}

void tryInfectCell(Cell* cell, Condition* condition){
	double infection_probabilty = condition->infection_probabilty;
	int gotInfected = randomUniformEvent(infection_probabilty);
	if (gotInfected){
		*cell = infected;
	}
}

int isInfected(Cell* cell){
	return *cell == infected;
}

int isCreature(Cell* cell){
	switch(*cell){
	case infected:
	case susceptible:
	case dead:
	case immune:
		return 1;
	case mountain:
	case store:
	case empty:
	default:
		return 0;
	}
	return 0;
}
