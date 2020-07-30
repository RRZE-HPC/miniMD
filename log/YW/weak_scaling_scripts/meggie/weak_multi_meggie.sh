#!/bin/bash -l
#
# allocate 40 nodes with 20 cores
#SBATCH --nodes=40
#SBATCH --tasks-per-node=20
#SBATCH --cpu-freq=2200000
#
# allocate nodes for 2 hours
#SBATCH --time=02:00:00
# job name 
# SBATCH --job-name=MiniMD_Meggie_Strong_single
# do not export environment variables
#SBATCH --export=NONE
#
# first non-empty non-comment line ends SBATCH options

# do not export environment variables
unset SLURM_EXPORT_ENV
# jobs always start in submit directory


module load intel64/19.0up05
module load intelmpi/2019up05-intel
module load likwid/5.0.1

x_init=16
y_init=16
z_init=16
echo "Initial size: x=$x | y=$y |z=$z"
echo " "

for ((j=800; j<=800; j+=20)); do 
    # Initial problem size
    x=$x_init
    y=$y_init
    z=$z_init
    count=0
    n_proc=$j
    declare -a factor_array

    i=2
    for ((i;i<=$n_proc;)); do
        if [ `expr $n_proc % $i` -eq 0 ];then
            factor=$i
            factor_array[count]=$factor
            count=`expr $count + 1`
            n_proc=$((n_proc / factor))
        else 
            i=`expr $i + 1`
        fi
    done
    # echo ${factor_array[@]}

    direction=1
    index=$((count-1))
    for ((index;index>=0;)); do
        if [ $direction -eq 1 ]; then
            x=$((x*${factor_array[index]}))
            direction=2
        elif [ $direction -eq 2 ]; then
            y=$((y*${factor_array[index]}))
            direction=3
        elif [ $direction -eq 3 ]; then
            z=$((z*${factor_array[index]}))
            direction=1
        else
            echo "FATAL ERROR"
        fi
        index=`expr $index - 1`
    done

    #printf "%3d processes:    x=%4d | y=%4d | z=%4d     Atoms: %d     Atoms/Proc: %d\n" $j $x $y $z $((4*x*y*z)) $((4*x*y*z/j))

    # call MiniMD here:
    mpirun -np "$j" -ppn 20 ../miniMD-ICC -nx "$x" -ny "$y" -nz "$z" -n 10000 >> res_weak_multi_meggie
done
