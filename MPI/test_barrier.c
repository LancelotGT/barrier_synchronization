#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include "gtmpi.h"

int main(int argc, char **argv) {
    char filename[20];
    int rank, size;
    FILE *fp;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    gtmpi_init(size);

    sprintf(filename, "file.out");
    fp = fopen(filename, "a");
    fprintf(fp, "Round 0 P%d\n", rank);
    fclose(fp);

    gtmpi_barrier();

    sprintf(filename, "file.out");
    fp = fopen(filename, "a");
    fprintf(fp, "Round 1 P%d\n", rank);
    fclose(fp); 

    gtmpi_barrier();

    sprintf(filename, "file.out");
    fp = fopen(filename, "a");
    fprintf(fp, "Round 2 P%d\n", rank);
    fclose(fp);  
    gtmpi_barrier(); 
    gtmpi_finalize();
    printf("rank %d finalize \n", rank);
    return 0;

}

