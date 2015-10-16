#include <omp.h>
#include "gtmp.h"
#include <stdlib.h>

/*
    From the MCS Paper: A sense-reversing centralized barrier

    shared count : integer := P
    shared sense : Boolean := true
    processor private local_sense : Boolean := true

    procedure central_barrier
        local_sense := not local_sense // each processor toggles its own sense
	if fetch_and_decrement (&count) = 1
	    count := P
	    sense := local_sense // last processor toggles global sense
        else
           repeat until sense = local_sense
*/

static int P;
static int count;
static int sense;
static int** localSense;

void gtmp_init(int num_threads){
    P = num_threads;
    count = num_threads;
    sense = 1;
    localSense = malloc(num_threads * sizeof(int*));

    int i;
    for (i = 0; i < num_threads; i++)
        posix_memalign((void*) &localSense[i], LEVEL1_DCACHE_LINESIZE, sizeof(int)); 
    for (i = 0; i < num_threads; i++)
        *localSense[i] = 1;
}

void gtmp_barrier(){
    int thread_num = omp_get_thread_num();
    *localSense[thread_num] = !(*localSense[thread_num]);

    if (__sync_fetch_and_sub(&count, 1) == 1)
    {   /* if it is the last processor */ 
        count = P;
        sense = *localSense[thread_num];
    }
    else
        while (sense != *localSense[thread_num]);
}

void gtmp_finalize(){
    int i = 0;
    for (i = 0; i < P; i++)
        free(localSense[i]);
    free(localSense);
}
