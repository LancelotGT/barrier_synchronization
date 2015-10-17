OMPFLAGS = -fopenmp -DLEVEL1_DCACHE_LINESIZE=`getconf LEVEL1_DCACHE_LINESIZE`
OMPLIBS = -lgomp

CC = gcc
CPPFLAGS = -g -Wall $(OMPFLAGS)
LDFLAGS = -g -Wall
LDLIBS = $(OMPLIBS)

all: test_openmp_counter test_openmp_mcs

test_openmp_counter: test_openmp.o gtmp_counter.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS) 

test_openmp_mcs: test_openmp.o gtmp_mcs.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)  

clean:
	rm -f *.o  test_openmp_counter test_openmp_mcs

clean_data:
	rm *.txt

test_counter: test_openmp_counter
	for n in 1 2 3 4 5 6 7 8 ; do \
		./test_openmp_counter $$n 100000 >> openmp_counter_result.txt; \
	done

test_mcs: test_openmp_mcs
	for n in 1 2 3 4 5 6 7 8 ; do \
		./test_openmp_mcs $$n 100000 >> openmp_mcs_result.txt; \
	done 
