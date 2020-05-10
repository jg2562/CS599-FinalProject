#include "parallel.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "queue.h"
#include "block.h"
#include "model.h"
#include "utils.h"

#define MPI_ENABLE 1

#if (MPI_ENABLE)
#include <omp.h>
#include <mpi.h>

static MPI_Datatype mpi_cell_type;
static MPI_Datatype mpi_cell_message_type;
static MPI_Datatype mpi_parameters_type;
static MPI_Datatype mpi_population_type;

#endif

static int total_ranks=1;
static int current_rank=0;
static int root_rank=0;

void setupRanks();
void setupDatatypes();
int getBlocksPerRank(Parameters* parameters);
int blockToRank(int block_index, Parameters* parameters);
void createCellDatatype();
void createCellMessageDatatype();
void createParameterDatatype();
void createPopulationDatatype();

void broadcastParameters(Parameters* parameters);
void scatterCellMap(CellMap** map, Parameters* parameters);
void broadcastPopulation(Population* population);

void gatherCellMap(CellMap* map, Parameters* parameters);
void sendCellMapToRoot(CellMessage* message, CellMap* map, Parameters* parameters);
void reducePopulation(Population* population);
void sendCellArrayToBlock(CellMessage* array, int count, int* block, Parameters* parameters, int to_rank);
void sendBlockToRank(CellMessage* send_cells, int send_count, CellMap* map, int* block, Parameters* parameters, int to_rank);


void parallelBegin(int* argc, char*** argv){
#if (MPI_ENABLE)
	MPI_Init(argc, argv);
	setupRanks();
	setupDatatypes();
#endif
}

void parallelEnd(){
#if (MPI_ENABLE)
	MPI_Finalize();
#endif
}

void setupRanks(){
#if (MPI_ENABLE)
	MPI_Comm_rank(MPI_COMM_WORLD,&current_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&total_ranks);
#endif
}

int isRootRank(){
	return current_rank == root_rank;
}

unsigned int getRank(){
	return current_rank;
}

unsigned int getTotalRanks(){
	return total_ranks;
}

Model* scatterAndInitializeModel(Model* model){
	if (model == NULL){
		model = createModel();
		model->parameters = createParameters();
		model->population = createPopulation();
	}
	scatterModel(model);
	return model;
}

void scatterModel(Model* model){
	broadcastParameters(model->parameters);
	scatterCellMap(&model->map, model->parameters);
	broadcastPopulation(model->population);
}

void broadcastParameters(Parameters* parameters){
#if (MPI_ENABLE)
	MPI_Bcast(parameters, 1, mpi_parameters_type, root_rank, MPI_COMM_WORLD);
#endif
}

void scatterCellMap(CellMap** map, Parameters* parameters){
	CellMessage* message = createCellMessage(parameters);
	int block_pos[2];
	int block_range[2];
	getLocalBlockRange(block_range, parameters);
	if (isRootRank()){
		//TODO Refactor the double for loop here
		for(int block_index = 0; block_index < block_range[0]; block_index++){
			blockIndexToPosition(block_pos, block_index, parameters);
			int send_count = getBlockArea(block_pos, parameters);
			sendBlock(message, send_count, *map, block_pos, parameters);
		}

		for(int block_index = block_range[1]; block_index < getTotalBlocks(parameters); block_index++){
			blockIndexToPosition(block_pos, block_index, parameters);
			int send_count = getBlockArea(block_pos, parameters);
			sendBlock(message, send_count, *map, block_pos, parameters);
		}
	} else {
		*map = createCellMap(parameters->model_width, parameters->model_height);
		for(int block_index = block_range[0]; block_index < block_range[1]; block_index++){
			receiveBlock(*map, message, parameters);
		}
	}
	freeCellMessage(message);
}

void broadcastPopulation(Population* population){
#if (MPI_ENABLE)
	MPI_Bcast(population, 1, mpi_population_type, root_rank, MPI_COMM_WORLD);
#endif
}

void getLocalBlockRange(int* blocks, Parameters* parameters){
	int block_count = getTotalBlocks(parameters);
	int block_per_rank = getBlocksPerRank(parameters);

	blocks[0] = block_per_rank * getRank();
	blocks[1] = min(block_per_rank * (getRank()+1), block_count);
}

void gatherModel(Model* model){
	gatherCellMap(model->map, model->parameters);
	reducePopulation(model->population);
}

void gatherCellMap(CellMap* map, Parameters* parameters){
	CellMessage* message = createCellMessage(parameters);
	if (isRootRank()){
		int block_range[2];
		getLocalBlockRange(block_range, parameters);
		int receive_count = getTotalBlocks(parameters) - (block_range[1] - block_range[0]);

		for(int i = 0; i < receive_count; i++){
			receiveBlock(map, message, parameters);
		}

	} else{
		sendCellMapToRoot(message, map, parameters);
	}
	freeCellMessage(message);
}

void sendCellMapToRoot(CellMessage* message, CellMap* map, Parameters* parameters){
	int block_pos[2];
	int block_range[2];
	getLocalBlockRange(block_range, parameters);
	for(int block_index = block_range[0]; block_index < block_range[1]; block_index++){
		blockIndexToPosition(block_pos, block_index, parameters);
		int send_count = getBlockArea(block_pos, parameters);
		sendBlockToRank(message, send_count, map, block_pos, parameters, root_rank);
	}
}

void reducePopulation(Population* population){
#if (MPI_ENABLE)
	int recv[4];
	recv[0] = population->susceptible;
	recv[1] = population->infected;
	recv[2] = population->dead;
	recv[3] = population->immune;
	int send[4] = {0,0,0,0};
	MPI_Reduce(recv, send, 4, MPI_INT, MPI_SUM, root_rank, MPI_COMM_WORLD);
	population->susceptible = send[0];
	population->infected = send[1];
	population->dead = send[2];
	population->immune = send[3];
#endif
}


int hasMessages(){
#if (MPI_ENABLE)
	MPI_Status status;
	int success;
	MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &success, &status);
	return success;
#else
	return 0;
#endif
}

void synchronize(){
#if (MPI_ENABLE)
	MPI_Barrier(MPI_COMM_WORLD);
#endif
}

void sendCellArrayToBlock(CellMessage* array, int count, int* block, Parameters* parameters, int to_rank){
#if (MPI_ENABLE)
	int block_index = positionToBlockIndex(block, parameters);
	CellMessage first;
	first.x = block_index;
	first.y = count;

	array[0] = first;

	MPI_Request request;
	MPI_Isend(array, count+1, mpi_cell_message_type, to_rank, 0, MPI_COMM_WORLD, &request);
	MPI_Request_free(&request);
#endif
}

void receiveCellArrayAtBlock(CellMessage* array, int* count, int* block, Parameters* parameters){
#if (MPI_ENABLE)
	int block_size = getMaxBlockArea(parameters);
	MPI_Status status;
	MPI_Recv(array, block_size+1, mpi_cell_message_type, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

	CellMessage first = array[0];
	int block_index = first.x;
	blockIndexToPosition(block, block_index, parameters);
	*count = first.y;
#endif
}

void fillSendCellsArray(CellMessage* send_cells, int send_count, CellMap* map, int* block, Parameters* parameters){
	int block_dims[2];
	int origin[2];
	getBlockDimensions(block_dims, block, parameters);
	getBlockOrigin(origin, block, parameters);

	// First cell is used for block into
	int index = 0;
	for (int row = 0; row < block_dims[1]; row++){
		for (int col = 0; col < block_dims[0]; col++){
			if (index >= send_count){
				break;
			}
			Cell* target = getCell(map, col+origin[0], row+origin[1]);
			CellMessage message;
			message.x = col;
			message.y = row;
			message.cell = *target;
			send_cells[index+1] = message;
			index++;
		}
	}

}

void dumpCellArrayToBlock(CellMap* map, CellMessage* receive_cells, int* block, int receive_count, Parameters* parameters){
#if (MPI_ENABLE)
	// First cell is used for block into
	CellMessage* message;
	int origin[2];
	getBlockOrigin(origin, block, parameters);
	int x;
	int y;
	Cell* target;
	for (int i = 0; i < receive_count; i++){
		message = &receive_cells[i+1];
		x = message->x + origin[0];
		y = message->y + origin[1];
		target = getCell(map, x, y);
		*target = message->cell;
	}
#endif
}

void sendBlock(CellMessage* send_cells, int send_count, CellMap* map, int* block, Parameters* parameters){
	int block_index = positionToBlockIndex(block, parameters);
	int to_rank = blockToRank(block_index,parameters);
	sendBlockToRank(send_cells, send_count, map, block, parameters, to_rank);
}
void sendBlockToRank(CellMessage* send_cells, int send_count, CellMap* map, int* block, Parameters* parameters, int to_rank){
	fillSendCellsArray(send_cells, send_count, map, block, parameters);
	sendCellArrayToBlock(send_cells, send_count, block, parameters, to_rank);
}

void receiveBlock(CellMap* map, CellMessage* receive_cells, Parameters* parameters){
	int block[2];
	int receive_count;
	receiveCellArrayAtBlock(receive_cells, &receive_count, block, parameters);
	dumpCellArrayToBlock(map, receive_cells, block, receive_count, parameters);
}

int blockToRank(int block_index, Parameters* parameters){
	int to_rank = block_index/getBlocksPerRank(parameters);
	return to_rank;
}

int getBlocksPerRank(Parameters* parameters){
	int total_blocks = getTotalBlocks(parameters);
	return total_blocks/total_ranks + (total_blocks % total_ranks != 0);
}

void setupDatatypes(){
#if (MPI_ENABLE)
	createCellDatatype();
	createCellMessageDatatype();
	createPopulationDatatype();
	createParameterDatatype();
#endif
}

void createCellDatatype(){
#if (MPI_ENABLE)
	int cell_blocklengths[2] = {1,1};
	MPI_Datatype cell_types[2] = {MPI_INT,MPI_UNSIGNED};
	MPI_Aint cell_offsets[2];
	cell_offsets[0] = offsetof(Cell, state);
	cell_offsets[1] = offsetof(Cell, state_start_time);

	MPI_Type_create_struct(2, cell_blocklengths, cell_offsets, cell_types, &mpi_cell_type);
	MPI_Type_commit(&mpi_cell_type);
#endif
}

void createCellMessageDatatype(){
#if (MPI_ENABLE)
	int cell_message_blocklengths[2] = {2,1};
	MPI_Datatype cell_message_types[2] = {MPI_INT,mpi_cell_type};
	MPI_Aint cell_message_offsets[2];
	cell_message_offsets[0] = offsetof(CellMessage, x);
	cell_message_offsets[1] = offsetof(CellMessage, cell);

	MPI_Type_create_struct(2, cell_message_blocklengths, cell_message_offsets, cell_message_types, &mpi_cell_message_type);
	MPI_Type_commit(&mpi_cell_message_type);
#endif
}

void createParameterDatatype(){
#if (MPI_ENABLE)
	int blocklengths[2] = {3,6};
	MPI_Datatype types[2] = {MPI_DOUBLE, MPI_INT};
	MPI_Aint offsets[2];
	offsets[0] = offsetof(Parameters, spread_rate);
	offsets[1] = offsetof(Parameters, model_height);

	MPI_Type_create_struct(2, blocklengths, offsets, types, &mpi_parameters_type);
	MPI_Type_commit(&mpi_parameters_type);
#endif
}

void createPopulationDatatype(){
#if (MPI_ENABLE)
	int blocklengths[1] = {4};
	MPI_Datatype types[1] = {MPI_INT};
	MPI_Aint offsets[1];
	offsets[0] = offsetof(Population, susceptible);

	MPI_Type_create_struct(1, blocklengths, offsets, types, &mpi_population_type);
	MPI_Type_commit(&mpi_population_type);
#endif
}

int validParallelParameters(Parameters* parameters){
	return (int) getTotalRanks() <= getTotalBlocks(parameters);
}

CellMessage* createCellMessage(Parameters* parameters){
	CellMessage* message = malloc(sizeof(*message) * (getMaxBlockArea(parameters)+1));
	return message;
}


void freeCellMessage(CellMessage* message){
	free(message);
}

