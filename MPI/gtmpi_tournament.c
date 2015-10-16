#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#include "gtmpi.h"

/*
    From the MCS Paper: A scalable, distributed tournament barrier with only local spinning

    type round_t = record
        role : (winner, loser, bye, champion, dropout)
	opponent : ^Boolean
	flag : Boolean
    shared rounds : array [0..P-1][0..LogP] of round_t
        // row vpid of rounds is allocated in shared memory
	// locally accessible to processor vpid

    processor private sense : Boolean := true
    processor private vpid : integer // a unique virtual processor index

    //initially
    //    rounds[i][k].flag = false for all i,k
    //rounds[i][k].role = 
    //    winner if k > 0, i mod 2^k = 0, i + 2^(k-1) < P , and 2^k < P
    //    bye if k > 0, i mode 2^k = 0, and i + 2^(k-1) >= P
    //    loser if k > 0 and i mode 2^k = 2^(k-1)
    //    champion if k > 0, i = 0, and 2^k >= P
    //    dropout if k = 0
    //    unused otherwise; value immaterial
    //rounds[i][k].opponent points to 
    //    round[i-2^(k-1)][k].flag if rounds[i][k].role = loser
    //    round[i+2^(k-1)][k].flag if rounds[i][k].role = winner or champion
    //    unused otherwise; value immaterial
    procedure tournament_barrier
        round : integer := 1
	loop   //arrival
	    case rounds[vpid][round].role of
	        loser:
	            rounds[vpid][round].opponent^ :=  sense
		    repeat until rounds[vpid][round].flag = sense
		    exit loop
   	        winner:
	            repeat until rounds[vpid][round].flag = sense
		bye:  //do nothing
		champion:
	            repeat until rounds[vpid][round].flag = sense
		    rounds[vpid][round].opponent^ := sense
		    exit loop
		dropout: // impossible
	    round := round + 1
	loop  // wakeup
	    round := round - 1
	    case rounds[vpid][round].role of
	        loser: // impossible
		winner:
		    rounds[vpid[round].opponent^ := sense
		bye: // do nothing
		champion: // impossible
		dropout:
		    exit loop
	sense := not sense
*/


typedef enum {DEFAULT, WINNER, LOSER, BYE, CHAMPION, DROPOUT} role_t;

typedef struct round_t {
	role_t role;
	int opponent;
} round_t;

static int P, num_rounds;
round_t** rounds;

static MPI_Status* status_array;


void gtmpi_init(int num_threads){
	P = num_threads;
	num_rounds = 0;
	while (P > (1 << num_rounds)) num_rounds++;
	rounds = (round_t**) malloc(P * sizeof(round_t*));
	status_array = (MPI_Status*) malloc(num_rounds * sizeof(MPI_Status));

	// initially 
	int vpid, i, k;
	MPI_Comm_rank(MPI_COMM_WORLD, &vpid);

	rounds[vpid] = (round_t*) malloc((num_rounds + 1) * sizeof(round_t));
	i = vpid;
	for (k = 0; k <= num_rounds; k++) {
		// role
		if (k == 0) rounds[i][k].role = DROPOUT;
		else if (i == 0 && 1 << k >= P) rounds[i][k].role = CHAMPION;
		else if (i % (1 << k) == 1 << (k - 1)) rounds[i][k].role = LOSER;
		else if (i % (1 << k) == 0 && i + (1 << (k - 1)) >= P) rounds[i][k].role = BYE;
		else if (i % (1 << k) == 0 && i + (1 << (k - 1)) < P && 1 << k < P) rounds[i][k].role = WINNER;
		else rounds[i][k].role = DEFAULT;
		// opponent 
		if (rounds[i][k].role == LOSER) 
			rounds[i][k].opponent = i - (1 << (k - 1));
		else if (rounds[i][k].role == WINNER || rounds[i][k].role == CHAMPION) {
			rounds[i][k].opponent = i + (1 << (k - 1));
		}
	}
	//printf("P: %d, num_rounds: %d\n", P, num_rounds);
}


void gtmpi_barrier(){
	int vpid, k;

  	MPI_Comm_rank(MPI_COMM_WORLD, &vpid);

  	//  arrival
  	for (k = 1; k <= num_rounds; k++) {
  		switch(rounds[vpid][k].role) {
  			case LOSER: 
  				MPI_Send(NULL, 0, MPI_INT, rounds[vpid][k].opponent, 1, MPI_COMM_WORLD);
  				MPI_Recv(NULL, 0, MPI_INT, rounds[vpid][k].opponent, 1, MPI_COMM_WORLD, &status_array[k - 1]);
  				break;
  			case WINNER:
  				MPI_Recv(NULL, 0, MPI_INT, rounds[vpid][k].opponent, 1, MPI_COMM_WORLD, &status_array[k - 1]);
  				break;
  			case CHAMPION: 
  				MPI_Recv(NULL, 0, MPI_INT, rounds[vpid][k].opponent, 1, MPI_COMM_WORLD, &status_array[k - 1]);
  				MPI_Send(NULL, 0, MPI_INT, rounds[vpid][k].opponent, 1, MPI_COMM_WORLD);
  				break;
  			default: 
  				break;
  		}
  	}
  	// wakeup
  	for (k = num_rounds - 1; k >= 0; k--) {
   		if (rounds[vpid][k].role == WINNER) 
   			MPI_Send(NULL, 0, MPI_INT, rounds[vpid][k].opponent, 1, MPI_COMM_WORLD);
  	}
}

void gtmpi_finalize(){
  if(status_array != NULL) free(status_array);
  if (rounds != NULL) free(rounds);
}







