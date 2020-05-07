#ifndef BLOCK_H
#define BLOCK_H
#include "parameters.h"

void getIterationBlocks(int* blocks, Parameters* parameters);
int segmentDimension(int length, int segmentation);
void blockIndexToPosition(int* block_pos, int block_index, Parameters* parameters);
int getDimensionLength(int block_index, int block_len, int map_size);

#endif /* BLOCK_H */
