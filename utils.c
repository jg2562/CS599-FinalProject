#include "utils.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void lowerCaseString(char* str){
	if (str == NULL){
		return;
	}

	int i = 0;
	while (str[i]){
		str[i] = tolower(str[i]);
		i++;
	}
}

double max(double a, double b){
	return a > b ? a : b;
}

double min(double a, double b){
	return a < b ? a : b;
}

char* duplicateString(const char* str){
	if (str == NULL){
		return NULL;
	}

	unsigned int len = strlen(str);
	char* copy = malloc(sizeof(*copy) * len+1);
	copy[len] = '\0';
	strcpy(copy, str);
	return copy;
}

void boundBox(int min_index[2], int max_index[2], int bound_min[2], int bound_max[2]){
	min_index[0] = max(min_index[0], bound_min[0]);
	min_index[1] = max(min_index[1], bound_min[1]);

	max_index[0] = min(max_index[0], bound_max[0]);
	max_index[1] = min(max_index[1], bound_max[1]);
}
