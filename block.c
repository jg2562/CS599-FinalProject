#include "block.h"

void getIterationBlocks(int* blocks, Parameters* parameters){
	int width = segmentDimension(parameters->model_width, parameters->block_width);
	int height = segmentDimension(parameters->model_height, parameters->block_height);
	int block_count = width*height;
	blocks[0] = 0;
	blocks[1] = block_count;
}

int segmentDimension(int length, int segmentation){
	int divided = length / segmentation;
	divided += (length % segmentation) != 0;
	return divided;
}

void blockIndexToPosition(int* block_pos, int block_index, Parameters* parameters){
	int width = segmentDimension(parameters->model_width, parameters->block_width);
	block_pos[0] = block_index % width;
	block_pos[1] = block_index / width;
}

int getDimensionLength(int block_index, int block_len, int map_size){
	int max_index = (block_index + 1) * block_len;
	return  max_index < map_size ? block_len : map_size - max_index + block_len;
}
