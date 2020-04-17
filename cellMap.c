#include "cellMap.h"
#include <stdlib.h>
#include <stdio.h>

CellMap* createCellMap(int width, int height){
	CellMap* map = malloc(sizeof(*map) * height);
	for (int i = 0; i < height; i++){
		map[i] = malloc(sizeof(**map) * width);
		for (int j = 0; j < width; j++){
			map[i][j] = createCell();
		}
	}
	return map;
}

void freeCellMap(CellMap* map, int height){
	for (int i = 0; i < height; i++){
		free(map[i]);
	}
	free(map);
}

void printCellMap(CellMap* map, int width, int height){
	for (int row = 0; row < height; row++){
		for (int col = 0; col < width; col++){
			printf("%c", cellToChar(map[row][col]));
		}
		printf("\n");
	}
}
