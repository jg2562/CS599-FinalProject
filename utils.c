#include "utils.h"
#include <ctype.h>

void lowerCaseString(char* str){

	int i = 0;
	while (str[i]){
		str[i] = tolower(str[i]);
		i++;
	}
}
