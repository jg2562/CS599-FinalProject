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

unsigned int cell_type_count = 7;
void transmissionEffect(Cell* target, Parameters* parameters, Condition* target_condition);
void tryInfectCell(Cell* cell, Condition* condition);
void tryCureCell(Cell* cell, Parameters* condition);

Cell createDefaultCell(){
	return createCell(unknown);
}

Cell createCell(CellState state){
	Cell cell;
	cell.state = state;
	cell.state_start_time = 0;
	return cell;
}

Cell charToCell(const char c){
	switch (c){
	case EMPTY_CHAR:
		return createCell(empty);
	case INFECTED_CHAR:
		return createCell(infected);
	case SUSCEPTIBLE_CHAR:
		return createCell(susceptible);
	case DEAD_CHAR:
		return createCell(dead);
	case IMMUNE_CHAR:
		return createCell(immune);
	case MOUNTAIN_CHAR:
		return createCell(mountain);
	case STORE_CHAR:
		return createCell(store);
	default:
		return createCell(unknown);
	}
}

char cellToChar(Cell cell){
	switch(cell.state){
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

Cell randomCell(){
	char choices[] = {EMPTY_CHAR, INFECTED_CHAR,
		SUSCEPTIBLE_CHAR, DEAD_CHAR, IMMUNE_CHAR,
		MOUNTAIN_CHAR, STORE_CHAR, DEFAULT_CHAR};

	int choice = randomInt(cell_type_count);

	return charToCell(choices[choice]);
}

double getEffectRadius(Cell* cell, Parameters* parameters){

	switch(cell->state){
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
	switch(cell->state){
	case infected:
		return 1;
	default:
		return 0;
	}
	return 0;
}

void applyCellEffect(Cell* cell, Cell* target, Parameters* parameters, Condition* target_condition){
	switch(cell->state){
	case infected:
		transmissionEffect(target,parameters,target_condition);
		break;
	default:
		break;
	}

}

void transmissionEffect(Cell* target, Parameters* parameters, Condition* target_condition){
	if (target->state == susceptible){
		target_condition->infection_probabilty += parameters->spread_rate*0.01;
	}
}

void applyConditionsToCell(Cell* cell, Parameters* parameters, Condition* condition, unsigned int time_step){
	switch(cell->state){
	case susceptible:
		tryInfectCell(cell, condition);
		break;
	case infected:
		tryCureCell(cell, parameters);
		break;
	default:
		break;
	}
}

void tryInfectCell(Cell* cell, Condition* condition){
	double infection_probabilty = condition->infection_probabilty;
	int gotInfected = randomUniformEvent(infection_probabilty);
	if (gotInfected){
		cell->state = infected;
	}
}

void tryCureCell(Cell* cell, Parameters* parameters){
	double recovery_probabilty = parameters->recovery_rate * 0.01;
	int gotCured = randomUniformEvent(recovery_probabilty);
	if (gotCured){
		cell->state = immune;
	} else {
		double death_probabilty = parameters->recovery_rate * 0.01;
		int gotKilled = randomUniformEvent(death_probabilty);
		if (gotKilled){
			cell->state = dead;
		}
	}
}
int isSusceptible(Cell* cell){
	return cell->state == susceptible;
}

int isInfected(Cell* cell){
	return cell->state == infected;
}

int isImmune(Cell* cell){
	return cell->state == immune;
}

int isDead(Cell* cell){
	return cell->state == dead;
}

int isCreature(Cell* cell){
	switch(cell->state){
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
