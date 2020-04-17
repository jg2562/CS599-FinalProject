
#include  <stdlib.h>
#include  <stdio.h>
#include "simulation.h"

int main(int argc, char** argv){
	if (argc != 3){
		fprintf(stderr, "===========================\n");
		fprintf(stderr, "Error: 2 parameters expected, %d were given.\n", argc);
		fprintf(stderr, "Proper Usage:\n");
		fprintf(stderr, "\t%s <parameter file> <map file>\n", argv[0]);
		fprintf(stderr, "===========================\n");
	}
	testSimulation(argv[1], argv[2]);

	exit(0);
}
