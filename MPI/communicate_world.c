#include <stdio.h>
#include "mpi.h"
#include "gtmpi.h"

int main(int argc, char **argv)
{
  int my_id, my_dst, my_src, num_processes;
  int tag = 1;
  int send_msg[2];
  int recv_msg[2];
  MPI_Status mpi_result;

  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  gtmpi_init(num_processes);

  my_dst = (my_id + 1) % num_processes;
  my_src = (my_id - 1) % num_processes;
  while (my_src < 0)
    my_src += num_processes;

  send_msg[0] = my_id;
  send_msg[1] = num_processes;

    // Be careful of deadlock when using blocking sends and receives!
  int i;
  for (i = 0; i < 3; i++) {
    MPI_Send(&send_msg, 2, MPI_INT, my_dst, tag, MPI_COMM_WORLD);
    MPI_Recv(&recv_msg, 2, MPI_INT, my_src, tag, MPI_COMM_WORLD, &mpi_result);

    printf("%d proc %d: received message from proc %d of %d\n", i, my_id, recv_msg[0], recv_msg[1]);
    fflush(stdout);
    //MPI_Barrier(MPI_COMM_WORLD);
    gtmpi_barrier();
  }

  gtmpi_finalize();

  MPI_Finalize();

  return 0;
}

