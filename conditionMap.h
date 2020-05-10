#ifndef CONDITIONMAP_H
#define CONDITIONMAP_H

#include "condition.h"

typedef Condition* ConditionMap;

Condition* getCondition(ConditionMap* map, int x, int y);

ConditionMap* createConditionMap(int width, int height);
void freeConditionMap(ConditionMap* map, int height);

#endif /* CONDITIONMAP_H */
