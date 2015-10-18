#PBS -q cs6210
#PBS -l nodes=jinx2+jinx3+jinx4+jinx5+jinx6+jinx7+jinx8+jinx9+jinx10+jinx11+jinx12+jinx13
#PBS -N test_MPI

export PBS_O_WORKDIR=$HOME/cs6210/proj2/barrier_synchronization/
RUN=/usr/lib64/openmpi/bin/mpirun
OUTPUT=$PBS_O_WORKDIR/mpi_result.txt

for algo in test_MPI_tournament test_MPI_dissemination
do
        EXE=$PBS_O_WORKDIR/MPI/$algo
        echo "==============  $algo  =============" >> $OUTPUT
        for node in 2 4 6 8 10 12
        do
                echo "result for $node nodes" >> $OUTPUT
                $RUN --hostfile $PBS_NODEFILE -np $node $EXE >> $OUTPUT
                echo " " >> $OUTPUT
        done
done