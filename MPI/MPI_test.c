#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
#include <sys/time.h>
#include "mpi.h"
#include "gtmpi.h"

#define ROUNDS 10000

int main(int argc, char **argv) {
    int rank, size, i;
    long* recv_buf;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    recv_buf = (long*) malloc(size * sizeof(long));

    gtmpi_init(size);

    //struct timespec start, end;
    struct timeval start, end;
    //clock_gettime(CLOCK_REALTIME, &start);
    gettimeofday(&start, NULL);
    for (i = 0; i < ROUNDS; i++) {
        gtmpi_barrier(); 
    }
    gettimeofday(&end, NULL);
    //clock_gettime(CLOCK_REALTIME, &end);

    //long avg_time = (end.tv_sec * 1000000000L + end.tv_nsec - (start.tv_sec * 1000000000L + start.tv_nsec)) / ROUNDS;
    long avg_time = ((end.tv_sec * 1000000L + end.tv_usec) - (start.tv_sec * 1000000L + start.tv_usec)) / ROUNDS;

    MPI_Gather(&avg_time, 1, MPI_LONG, recv_buf, 1, MPI_LONG, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        long sum_avg_time = 0;
        for (i = 0; i < size; i++) sum_avg_time += recv_buf[i];
        printf("%d\t%ld\n", size, sum_avg_time / size);
    }

    free(recv_buf);
    gtmpi_finalize();
    MPI_Finalize();

    return 0;
}

