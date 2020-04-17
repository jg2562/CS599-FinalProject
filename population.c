#include "population.h"
#include <stdlib.h>
#include <stdio.h>

Population* createPopulation(){
	Population* population = malloc(sizeof(*population));
	population->susceptible = -1;
	population->infected = -1;
	population->dead = -1;
	population->immune = -1;
	return population;
}

void freePopulation(Population* population){
	free(population);
}

void initializePopulation(Population* population){
	population->susceptible = 0;
	population->infected = 0;
	population->dead = 0;
	population->immune = 0;
}
