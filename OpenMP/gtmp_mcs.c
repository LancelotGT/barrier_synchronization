#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>
#include "gtmp.h"

/*
    From the MCS Paper: A scalable, distributed tree-based barrier with only local spinning.

    type treenode = record
        parentsense : Boolean
	parentpointer : ^Boolean
	childpointers : array [0..1] of ^Boolean
	havechild : array [0..3] of Boolean
	childnotready : array [0..3] of Boolean
	dummy : Boolean //pseudo-data

    shared nodes : array [0..P-1] of treenode
        // nodes[vpid] is allocated in shared memory
        // locally accessible to processor vpid
    processor private vpid : integer // a unique virtual processor index
    processor private sense : Boolean

    // on processor i, sense is initially true
    // in nodes[i]:
    //    havechild[j] = true if 4 * i + j + 1 < P; otherwise false
    //    parentpointer = &nodes[floor((i-1)/4].childnotready[(i-1) mod 4],
    //        or dummy if i = 0
    //    childpointers[0] = &nodes[2*i+1].parentsense, or &dummy if 2*i+1 >= P
    //    childpointers[1] = &nodes[2*i+2].parentsense, or &dummy if 2*i+2 >= P
    //    initially childnotready = havechild and parentsense = false
	
    procedure tree_barrier
        with nodes[vpid] do
	    repeat until childnotready = {false, false, false, false}
	    childnotready := havechild //prepare for next barrier
	    parentpointer^ := false //let parent know I'm ready
	    // if not root, wait until my parent signals wakeup
	    if vpid != 0
	        repeat until parentsense = sense
	    // signal children in wakeup tree
	    childpointers[0]^ := sense
	    childpointers[1]^ := sense
	    sense := not sense
*/

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
static int P;

void gtmp_init(int num_threads){
    P = num_threads;
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
            nodes[i].haveChild[j] = (4 * i + j + 1 < P) ? 1 : 0; /* init haveChild */

        nodes[i].parentPointer = (i == 0) ? &nodes[i].dummy : &nodes[(i - 1) / 4].childNotReady[(i - 1) % 4]; 
        nodes[i].childPointers[0] = (2 * i + 1 >= P) ? &nodes[i].dummy : &nodes[2 * i + 1].parentSense;
        nodes[i].childPointers[1] = (2 * i + 2 >= P) ? &nodes[i].dummy : &nodes[2 * i + 2].parentSense; 

        for (j = 0; j < 4; j++)
            nodes[i].childNotReady[j] = nodes[i].haveChild[j];

        nodes[i].parentSense = 0; 
    }
}

void gtmp_barrier(){
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

    /* wake up */
    *nodes[thread_num].childPointers[0] = nodes[thread_num].sense;
    *nodes[thread_num].childPointers[1] = nodes[thread_num].sense; 
    nodes[thread_num].sense = !(nodes[thread_num].sense);
}

void gtmp_finalize(){
    free(nodes);
}
