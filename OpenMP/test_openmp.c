#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "gtmp.h"
#include <sys/time.h>

#define DEFAULT_N_THREADS 4
#define DEFAULT_N_ROUNDS 100000

int main(int argc, char **argv)
{
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
    gtmp_init(num_threads);

    double time_array[num_threads]; /* used to record the timer result for each thread */

#pragma omp parallel
    {
        int thread_num = omp_get_thread_num();
        struct timeval start, end; 
        int i;

        /* start measuring the barrier time */
        gettimeofday(&start, NULL);
        for (i = 0; i < rounds; i++)
            gtmp_barrier(); 
        gettimeofday(&end, NULL);  

        long total_time = (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec);
        double avg_time = total_time / (double) rounds;
        time_array[thread_num] = avg_time;
    }

    gtmp_finalize();
    
    /* calculate avg result from all threads */
    double sum = 0;
    int i;
    for (i = 0; i < num_threads; i++)
        sum += time_array[i];

    /* output result */
    printf("%d\t%f\n", num_threads, sum / num_threads);

    return 0;
}

