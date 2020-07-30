#!/bin/bash

cd $PBS_O_WORKDIR

module load intel64/19.0up05
module load likwid/5.0.1

for i in {1..40}; do
    for k in {1..3}; do
        n=$((20*i))
        likwid-mpirun -np "$n" -nperdomain S:10 ../miniMD-ICC --half_neigh 0 -s 20 -n 10000 >> result_32
        likwid-mpirun -np "$n" -nperdomain S:10 ../miniMD-ICC --half_neigh 0 -n 10000 >> result_131
        likwid-mpirun -np "$n" -nperdomain S:10 ../miniMD-ICC --half_neigh 0 -s 40 -n 10000 >> result_256
    done
done

