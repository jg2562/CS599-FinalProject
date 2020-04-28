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
