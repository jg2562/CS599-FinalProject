#ifndef CONDITIONMAP_H
#define CONDITIONMAP_H

#include "condition.h"

typedef Condition* ConditionMap;

ConditionMap* createConditionMap(int width, int height);
void freeConditionMap(ConditionMap* map, int height);

#endif /* CONDITIONMAP_H */
