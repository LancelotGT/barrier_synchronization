#PBS -q cs6210
#PBS -l nodes=8:sixcore
#PBS -N Ning

export PBS_O_WORKDIR=$HOME/barrier_synchronization/Combined
EXE=$PBS_O_WORKDIR/test_combined
OUTPUT=$PBS_O_WORKDIR/combined_result.txt
RUN = /usr/lib64/openmpi/bin/mpirun

for NP in 2 3 4 5 6 7 8
do
    echo "Number of Processes: $NP" >> $OUTPUT
    for nthreads in 2 4 6 8 10 12
    do 
        echo "Number of Threads: $nthreads">> $OUTPUT
        $(RUN) --hostfile $PBS_NODEFILE -np $NP $EXE $omp >> $OUTPUT
        echo " " >> $OUTPUT
    done
done
