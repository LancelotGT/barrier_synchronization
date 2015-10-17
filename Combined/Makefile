OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = gcc
CPPFLAGS = -g -Wall $(OMPFLAGS)
LDFLAGS = -g
LDLIBS = $(OMPLIBS)

MPICC = mpicc
MPICH = /usr/lib/openmpi
CFLAGS = -I$(MPICH)/include

all: combined_test

combined_test: combined_test.o gtcombined.o gtmpi_dissemination.o
	$(MPICC) -o $@ $(CPPFLAGS) $^ $(LDLIBS) 

gtcombined.o: gtcombined.c
	$(CC) $(CPPFLAGS) -c $^

gtmpi_dissemination.o: gtmpi_dissemination.c 
	$(MPICC) -c $^ $(CPPFLAGS)

combined_test.o: combined_test.c 
	$(MPICC) -c $^ $(CPPFLAGS)
clean:
	rm -f *.o combined_test
