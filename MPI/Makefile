OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = mpicc
CPPFLAGS = -g -Wall
LDFLAGS = -g -Wall
LDLIBS = $(OMPLIBS)

MPICC = mpicc
MPICH = /usr/lib/openmpi
CFLAGS = -I$(MPICH)/include

all: communicate_world_tournament communicate_world_dissemination communicate_world_counter communicate_world_default 

communicate_world_tournament: communicate_world.o gtmpi_tournament.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

communicate_world_dissemination: communicate_world.o gtmpi_dissemination.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

communicate_world_counter: communicate_world.o gtmpi_counter.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

communicate_world_default: communicate_world.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

barrier_test: barrier_test.o gtmpi_counter.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS) 
clean:
	rm -f *.o 
