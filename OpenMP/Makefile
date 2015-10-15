OMPFLAGS = -fopenmp -DLEVEL1_DCACHE_LINESIZE=`getconf LEVEL1_DCACHE_LINESIZE`
OMPLIBS = -lgomp

CC = gcc
CPPFLAGS = -g -Wall $(OMPFLAGS)
LDFLAGS = -g -Wall
LDLIBS = $(OMPLIBS)

all: hello_world priv_vars shared_vars parallel_for shared_vars_gtmp shared_vars_mcs

hello_world: hello_world.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

priv_vars: priv_vars.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

shared_vars: shared_vars.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

shared_vars_gtmp: shared_vars_gtmp.o gtmp_counter.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS) 

shared_vars_mcs: shared_vars_gtmp.o gtmp_mcs.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)  

parallel_for: parallel_for.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f *.o hello_world priv_vars shared_vars parallel_for
