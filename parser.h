#ifndef PARSER_H
#define PARSER_H

#include "model.h"
#include "parameters.h"

Model* importModel(const char* parameter_file, const char* map_file);
Parameters* importParameters(const char* fname);
CellMap* importCellMap(const char* map_file, int width, int height);

#endif /* PARSER_H */
