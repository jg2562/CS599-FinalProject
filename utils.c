#include "utils.h"
#include <stdlib.h>
#include <ctype.h>

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
