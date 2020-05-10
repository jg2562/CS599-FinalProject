#include "conditionMap.h"

#include <stdlib.h>

#include "condition.h"

inline Condition* getCondition(ConditionMap* map, int x, int y){
	return &map[y][x];
}

ConditionMap* createConditionMap(int width, int height){
	ConditionMap* map = malloc(sizeof(*map) * height);
	for (int i = 0; i < height; i++){
		map[i] = malloc(sizeof(**map) * width);
		for (int j = 0; j < width; j++){
			map[i][j] = createCondition();
		}
	}
	return map;
}

void freeConditionMap(ConditionMap* map, int height){
	for (int i = 0; i < height; i++){
		free(map[i]);
	}
	free(map);
}
