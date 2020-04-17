#ifndef POPULATION_H
#define POPULATION_H

typedef struct Population{
	int susceptible;
	int infected;
	int dead;
	int immune;
} Population;

Population* createPopulation();
void freePopulation(Population* population);
void initializePopulation(Population* population);


#endif /* POPULATION_H */
