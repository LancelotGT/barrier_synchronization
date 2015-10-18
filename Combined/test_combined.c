#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "gtcombined.h"
#include <omp.h>

#define DEFAULT_N_THREADS 4
#define DEFAULT_N_ROUNDS 10

int main(int argc, char **argv) {
    int rank, size;
    int num_threads = DEFAULT_N_THREADS;
    int rounds = DEFAULT_N_ROUNDS;

    if (argc > 3)
    {
        printf("usage: ./test_openmp [nthreads] [rounds]\n");
        exit(1);
    }

    if (argc >= 2)
        num_threads = atoi(argv[1]);
    if (argc == 3)
        rounds = atoi(argv[2]);

    omp_set_num_threads(num_threads);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    gtcombined_init(size, num_threads);
    double time_array[num_threads];

#pragma omp parallel
    {
        int thread_num = omp_get_thread_num();
        struct timeval start, end;
        int i;
        
        /* start measuring the barrier time */
        gettimeofday(&start, NULL);
        for (i = 0; i < rounds; i++)
            gtcombined_barrier();
        gettimeofday(&end, NULL);

        long total_time = (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.    tv_usec);
        double avg_time = total_time / (double) rounds;
        time_array[thread_num] = avg_time;
    }

    /* calculate avg result from all threads */
    double sum = 0;
    int i;
    for (i = 0; i < num_threads; i++)
        sum += time_array[i];
    double avg_time = sum / num_threads;

    /* calculate avg result from all node */
    double* recv_buf = (double*) malloc(size * sizeof(double));
    MPI_Gather(&avg_time, 1, MPI_DOUBLE, recv_buf, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double sum_avg_time = 0;
        for (i = 0; i < size; i++) sum_avg_time += recv_buf[i];
        printf("%d\t%f\n", size, sum_avg_time / size);
    }

    free(recv_buf);
    gtcombined_finalize();    
    MPI_Finalize();
    return 0;
}

