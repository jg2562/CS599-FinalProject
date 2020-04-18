#include "randomMap.h"
#include "parameters.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>


void generateRandomMap(const char * parameter_file, const char * map_file)
{
  FILE *map_p = fopen(map_file, "w");
  FILE *param_p = fopen(parameter_file, "r");

  if (!map_p) {
		fprintf(stderr,"Unable to open map file\n");
	}
  else if (!param_p)
  {
    fprintf(stderr, "Unable to open parameter file\n");
  }

  Parameters * params = importParameters(parameter_file);

  int width = params -> model_width;
  int height= params -> model_height;
  int numchar = 7;
  int randomNumber;
  srand(params -> seed);

  for(int i = 0; i < height; i++)
  {
    for(int j = 0; j < width; j++)
    {
      randomNumber = rand() % numchar;
      switch(randomNumber)
      {
        case 0: fprintf(map_p, " ");
        break;
        case 1: fprintf(map_p, "o");
        break;
        case 2: fprintf(map_p, ".");
        break;
        case 3: fprintf(map_p, "X");
        break;
        case 4: fprintf(map_p, "I");
        break;
        case 5: fprintf(map_p, "M");
        break;
        case 6: fprintf(map_p, "@");
        break;
      }
    }
    fprintf(map_p, "\n");
  }

  fclose(map_p);
}
