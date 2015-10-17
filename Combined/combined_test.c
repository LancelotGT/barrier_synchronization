#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include "gtcombined.h"
#include <omp.h>

int main(int argc, char **argv) {
    char filename[20];
    int rank, size;
    FILE *fp;
    int thread_num = -1;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    gtcombined_init(size, 4);
    sprintf(filename, "file.out");
    //fp = fopen(filename, "a"); 

#pragma omp parallel num_threads(4) firstprivate(thread_num)
    {
        thread_num = omp_get_thread_num();
        //printf("P%d thread%d\n", rank, thread_num); 
        
#pragma omp critical
        {
            fp = fopen(filename, "a");
            fprintf(fp, "Round 0 P%d thread%d\n", rank, thread_num);
            fclose(fp);
        }

        gtcombined_barrier(); 

        //printf("After first barrier P%d thread%d\n", rank, thread_num);  
#pragma omp critical
        {
            fp = fopen(filename, "a");
            fprintf(fp, "Round 1 P%d thread%d\n", rank, thread_num);
            fclose(fp);
        } 

        gtcombined_barrier();
        //printf("After second barrier P%d thread%d\n", rank, thread_num);   
        
#pragma omp critical
        {
            fp = fopen(filename, "a");
            fprintf(fp, "Round 2 P%d thread%d\n", rank, thread_num);
            fclose(fp);
        }        

    }
    //fclose(fp);  
    gtcombined_finalize();  
    return 0;
}

