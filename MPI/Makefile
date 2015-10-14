OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

# CC = gcc
CC = mpicc
CPPFLAGS = -g -Wall
LDFLAGS = -g -Wall
LDLIBS = $(OMPLIBS)

MPICC = mpicc
MPICH = /usr/lib64/openmpi/1.4-gcc
CFLAGS = -I$(MPICH)/include

all: hello_world communicate_world openmp_mpi

hello_world: hello_world.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

communicate_world: communicate_world.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

openmp_mpi: openmp_mpi.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS) $(OMPLIBS)

openmp_mpi.o: openmp_mpi.c
	$(MPICC) -c -o $@ $(CPPFLAGS) $(CFLAGS) $(OMPFLAGS) $^

clean:
	rm -f *.o hello_world communicate_world openmp_mpi