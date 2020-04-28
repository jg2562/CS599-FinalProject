#ifndef PARALLEL_H
#define PARALLEL_H

void parallelBegin();
void parallelEnd();

unsigned int getRank();
unsigned int getTotalRanks();

unsigned int receiveIntMessage(int** message, unsigned int size, unsigned int rank_from);
void sendIntMessage(int* message, unsigned int size, unsigned int rank_to);

#endif /* PARALLEL_H */
