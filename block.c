#include "block.h"
#include "utils.h"

int segmentDimension(int length, int segmentation);
int getDimensionLength(int block_index, int block_len, int map_size);

void getBlockOrigin(int* origin, int* block_pos, Parameters* parameters){
	int max_width = parameters->model_width;
	int max_height = parameters->model_height;
	int width = parameters->block_width;
	int height = parameters->block_height;

	origin[0] = min(block_pos[0] * width, max_width);
	origin[1] = min(block_pos[1] * height, max_height);
}

void getBlockTerminus(int* terminus, int* block_pos, Parameters* parameters){
	int max_width = parameters->model_width;
	int max_height = parameters->model_height;
	int width = parameters->block_width;
	int height = parameters->block_height;

	terminus[0] = min((block_pos[0] + 1) * width, max_width);
	terminus[1] = min((block_pos[1] + 1) * height, max_height);
}

void getBlockDimensions(int* dimensions, int* block, Parameters* parameters){
	dimensions[0] = getDimensionLength(block[0], parameters->block_width, parameters->model_width);
	dimensions[1] = getDimensionLength(block[1], parameters->block_height, parameters->model_height);
}

void blockIndexToPosition(int* block_pos, int block_index, Parameters* parameters){
	int block_map_width = segmentDimension(parameters->model_width, parameters->block_width);

	block_pos[0] = block_index % block_map_width;
	block_pos[1] = block_index / block_map_width;
}

void globalPositionToBlock(int* block, int* global_pos, Parameters* parameters){
	int width = parameters->block_width;
	int height = parameters->block_height;

	block[0] = global_pos[0] / width;
	block[1] = global_pos[1] / height;

}

void getMapDimensionsInBlocks(int* dimensions, Parameters* parameters){
	dimensions[0] = segmentDimension(parameters->block_width, parameters->model_width);
	dimensions[1] = segmentDimension(parameters->block_height, parameters->model_height);
}

void getIterationBlocks(int* blocks, Parameters* parameters){
	int block_count = getTotalBlocks(parameters);
	blocks[0] = 0;
	blocks[1] = block_count;
}

int getDimensionLength(int block_index, int block_len, int map_size){
	int max_index = (block_index + 1) * block_len;
	if (max_index < map_size){
		return block_len;
	} else {
		return map_size - max_index + block_len;
	}
}

int segmentDimension(int length, int segmentation){
	int divided = length / segmentation;
	divided += (length % segmentation) != 0;
	return divided;
}

void localIndexToPosition(int* local_position, int index, int* block, Parameters* parameters){
	int block_dim[2];
	getBlockDimensions(block_dim, block, parameters);

	local_position[0] = index % block_dim[0];
	local_position[1] = index / block_dim[0];
}

int getTotalBlocks(Parameters* parameters){
	int width = segmentDimension(parameters->model_width, parameters->block_width);
	int height = segmentDimension(parameters->model_height, parameters->block_height);

	return width*height;
}
