#include <stdio.h>
#include <mpi.h>


int main(int argc, char **argv)
{
  int my_rank, nprocs;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);


  MPI_Finalize();
  return 0;
}
