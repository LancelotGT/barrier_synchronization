#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    char filename[20];
    int rank, size;
    FILE *fp;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank < 2) { /* proc 0 and 1 only */ 
        sprintf(filename, "file_%d.out", rank);
        fp = fopen(filename, "w");
        fprintf(fp, "P%d: before Barrier0\n", rank);
        fclose(fp);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank < 2) { /* proc 0 and 1 only */ 
        sprintf(filename, "file_%d.out", (rank==0)?1:0 );
        fp = fopen(filename, "a");
        fprintf(fp, "P%d: after Barrier0\n", rank);
        fclose(fp);
    }

    MPI_Barrier(MPI_COMM_WORLD); 

    if (rank < 2) { /* proc 0 and 1 only */ 
        sprintf(filename, "file_%d.out", rank);
        fp = fopen(filename, "a");
        fprintf(fp, "P%d: before Barrier2\n", rank);
        fclose(fp);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank < 2) { /* proc 0 and 1 only */ 
        sprintf(filename, "file_%d.out", (rank==0)?1:0 );
        fp = fopen(filename, "a");
        fprintf(fp, "P%d: after Barrier2\n", rank);
        fclose(fp);
    } 

    MPI_Barrier(MPI_COMM_WORLD); 

    if (rank < 2) { /* proc 0 and 1 only */ 
        sprintf(filename, "file_%d.out", rank);
        fp = fopen(filename, "a");
        fprintf(fp, "P%d: before Barrier3\n", rank);
        fclose(fp);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank < 2) { /* proc 0 and 1 only */ 
        sprintf(filename, "file_%d.out", (rank==0)?1:0 );
        fp = fopen(filename, "a");
        fprintf(fp, "P%d: after Barrier3\n", rank);
        fclose(fp);
    } 
    MPI_Finalize();
    return 0;

}

