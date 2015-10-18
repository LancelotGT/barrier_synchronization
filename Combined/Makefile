OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = gcc
CPPFLAGS = -g -Wall $(OMPFLAGS)
LDFLAGS = -g
LDLIBS = $(OMPLIBS)

MPICC = mpicc
MPICH = /usr/lib/openmpi
CFLAGS = -I$(MPICH)/include

all: test_combined

test_combined: test_combined.o gtcombined.o gtmpi_dissemination.o
	$(MPICC) -o $@ $(CPPFLAGS) $^ $(LDLIBS) 

gtcombined.o: gtcombined.c
	$(CC) $(CPPFLAGS) -c $^

gtmpi_dissemination.o: gtmpi_dissemination.c 
	$(MPICC) -c $^ $(CPPFLAGS)

test_combined.o: test_combined.c 
	$(MPICC) -c $^ $(CPPFLAGS)

clean:
	rm -f *.o test_combined
