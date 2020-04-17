#include "cell.h"
#include "parameters.h"

#define EMPTY_CHAR '_'
#define INFECTED_CHAR 'I'
#define SUSCEPTIBLE_CHAR 's'
#define DEAD_CHAR 'X'
#define IMMUNE_CHAR 'g'
#define MOUNTAIN_CHAR 'M'
#define STORE_CHAR '@'
#define DEFAULT_CHAR '?'

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
