#ifndef SIMULATION_H
#define SIMULATION_H

#include "parser.h"
#include "model.h"

void testSimulation(const char* parameter_file, const char* map_file);
void runSimulation(Model* model);
void runSimulationIterator(Model* model, void(*iteratorFunction) (Model*));

#endif /* SIMULATION_H */
