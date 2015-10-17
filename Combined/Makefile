OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = mpicc
CPPFLAGS = -g -Wall $(OMPFLAGS)
LDFLAGS = -g
LDLIBS = $(OMPLIBS)

MPICC = mpicc
MPICH = /usr/lib/openmpi
CFLAGS = -I$(MPICH)/include

all: combined_test

combined_test: combined_test.o gtcombined.o gtmpi_dissemination.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS) 

gtcombined.o: gtcombined.c
	gcc -g -Wall -fopenmp -c -o gtcombined.o gtcombined.c

clean:
	rm -f *.o 
