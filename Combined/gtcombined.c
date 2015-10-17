#include <stdlib.h>
#include <stdio.h>
#include "gtmpi.h"
#include "gtmp.h"

typedef struct treeNode_t {
    int parentSense;
    int* parentPointer; 
    int* childPointers[2]; 
    int haveChild[4];
    int childNotReady[4];
    int sense;
    int vpid;
    int dummy; /* represent a dummy node, may point to it */
} treeNode;

static treeNode* nodes; /* global shared nodes */
static int nthreads;

void gtcombined_init(int num_processes, int num_threads){
    gtmpi_init(num_processes);
    
    nthreads = num_threads;
    nodes = malloc(num_threads * sizeof(treeNode));

    /* initialize barrier */
    int i;
    for (i = 0; i < num_threads; i++)
    {
        nodes[i].sense = 1;
        nodes[i].vpid = i;
        nodes[i].dummy = 0;
        int j;
        for (j = 0; j < 4; j++)
            nodes[i].haveChild[j] = (4 * i + j + 1 < nthreads) ? 1 : 0; /* init haveChild */

        nodes[i].parentPointer = (i == 0) ? &nodes[i].dummy : &nodes[(i - 1) / 4].childNotReady[(i - 1) % 4]; 
        nodes[i].childPointers[0] = (2 * i + 1 >= nthreads) ? &nodes[i].dummy : &nodes[2 * i + 1].parentSense;
        nodes[i].childPointers[1] = (2 * i + 2 >= nthreads) ? &nodes[i].dummy : &nodes[2 * i + 2].parentSense; 

        for (j = 0; j < 4; j++)
            nodes[i].childNotReady[j] = nodes[i].haveChild[j];

        nodes[i].parentSense = 0; 
    }
}

void gtcombined_barrier(){
    int thread_num = omp_get_thread_num();
    /* spin until all childNotReady are false */

    while (nodes[thread_num].childNotReady[0] || nodes[thread_num].childNotReady[1] ||
            nodes[thread_num].childNotReady[2] || nodes[thread_num].childNotReady[3]);

    /* reinit for next barrier */
    int i;
    for (i = 0; i < 4; i++)
        nodes[thread_num].childNotReady[i] = nodes[thread_num].haveChild[i];

    *nodes[thread_num].parentPointer = 0; /* let parent know I'm ready */
    
    if (nodes[thread_num].vpid != 0)
        while (nodes[thread_num].parentSense != nodes[thread_num].sense);
    else
        gtmpi_barrier();

    /* wake up */
    *nodes[thread_num].childPointers[0] = nodes[thread_num].sense;
    *nodes[thread_num].childPointers[1] = nodes[thread_num].sense; 
    nodes[thread_num].sense = !(nodes[thread_num].sense);
}

void gtcombined_finalize(){
    free(nodes);
}
