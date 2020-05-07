#ifndef BLOCK_H
#define BLOCK_H
#include "parameters.h"

void getIterationBlocks(int* blocks, Parameters* parameters);
void blockIndexToPosition(int* block_pos, int block_index, Parameters* parameters);
void getBlockOrigin(int* origin, int* block_pos, Parameters* parameters);
void getBlockTerminus(int* terminus, int* block_pos, Parameters* parameters);
void getBlockDimensions(int* dimensions, int* block, Parameters* parameters);
void getMapDimensionsInBlocks(int* dimensions, Parameters* parameters);
void blockIndexToPosition(int* block_pos, int block_index, Parameters* parameters);
void globalPositionToBlock(int* block, int* global_pos, Parameters* parameters);
void localIndexToPosition(int* local_position, int index, int* block, Parameters* parameters);

#endif /* BLOCK_H */
