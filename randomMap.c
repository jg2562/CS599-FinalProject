#include "randomMap.h"
#include "parameters.h"
#include "cellMap.h"
#include "cell.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

void fillCellMapRandomly(Parameters* parameters, CellMap* map);

CellMap* generateRandomMap(Parameters* parameters) {

	int width = parameters -> model_width;
	int height= parameters -> model_height;

	CellMap* map = createCellMap(width, height);

	fillCellMapRandomly(parameters, map);

	return map;
}


void fillCellMapRandomly(Parameters* parameters, CellMap* map){
	int width = parameters -> model_width;
	int height= parameters -> model_height;

	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			Cell cell = randomCell();
			Cell* loc = getCell(map, j, i);
			*loc = cell;
		}
	}
}
