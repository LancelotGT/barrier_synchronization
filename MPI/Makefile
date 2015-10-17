OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = mpicc
CPPFLAGS = -g -Wall
LDFLAGS = -g -Wall
LDLIBS = $(OMPLIBS)

MPICC = mpicc
MPICH = /usr/lib/openmpi
CFLAGS = -I$(MPICH)/include

 all: test_MPI_tournament test_MPI_dissemination

test_MPI_tournament: MPI_test.o gtmpi_tournament.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

test_MPI_dissemination: MPI_test.o gtmpi_dissemination.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

test_tournament: test_MPI_tournament
	for n in 2 4 6 8 10 12 ; do \
		mpirun -np $$n ./test_MPI_tournament >> MPI_tournament_result.txt; \
	done 

test_dissemination: test_MPI_dissemination
	for n in 2 4 6 8 10 12 ; do \
		mpirun -np $$n ./test_MPI_dissemination >> MPI_dissemination_result.txt; \
	done 


clean:
	rm -f *.o 
