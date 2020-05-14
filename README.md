# How to build miniMD

* Edit `Makefile` and adjust the compiler used by editing the TAG variable. You want to always use the ICC tag.
* Edit `include_<TAG>.mk` to adjust build flags.
* Load modules:
`module load intel64/18.0up04`
`module load likwid/5.0-dev`

* Call make: `$ make` (you can ignore the warnings about unknown omp pragmas)
* To build with command line output use: `$ make Q=`
* To clean intermediate build results call: `$ make clean`
* To clean all build results call: `$ make distclean`
* To generate assembler output (the asm files will be in ICC directory): `$ make asm`

To enable LIKWID Marker instrumentation you need to set the `LIKWID_PERFMON` define in `include_ICC.mk`:
`DEFINES  = -D_GNU_SOURCE -DNOCHUNK -DUSE_SIMD -DLIKWID_PERFMON`

# MiniMD Options

Command line arguments overwrite the input file options.
To get available options call:
* `$ ../miniMD-ICC -h`

Useful options:
* Use halfneigh Lennard-Jones variant:
`../miniMD-ICC  --half_neigh 1`

* Use fullneigh Lennard-Jones variant:
`../miniMD-ICC  --half_neigh 0`

* Do more timesteps (default in input file is 100):
`../miniMD-ICC  -n 300`

* Output extended yaml output also to stdout:
`../miniMD-ICC --yaml_screen -o 1`


# How to run miniMD sequential

To run you also need to load above Intel and Likwid modules!

Run miniMD with default config file (lennard jones potential) `in.lj.miniMD`:
* `$ cd data`
* `$ ../miniMD-ICC`

Run miniMD with eam potential:
* `$ cd data`
* `$ ../miniMD-ICC`

Example with pinning using `likwid-pin`:
`$ likwid-pin -c S0:2 ../miniMD-ICC`

Example with profiling using `likwid-perfctr`:
`$ likwid-perfctr -C S0:2 -g FLOPS_DP -m  ../miniMD-ICC`


# How to run MPI parallel (on Meggie or Emmy)

To run you also need to load above Intel and Likwid modules!

## Emmy
Run miniMD with default config file (lennard jones potential) `in.lj.miniMD`:
* Request interactive job on Emmy with fixed frequency and likwid profiling:
`$ qsub -I -l nodes=1:ppn=40:f2.2:likwid  -l walltime=00:30:00`

* Load required modules:
`module load intel64` (may use specific version)
`module load likwid`

* Run benchmark:
`cd ./data`
`likwid-mpirun -np 4 -mpi intelmpi ../miniMD-ICC`
`likwid-mpirun -np 4 -mpi intelmpi ../miniMD-ICC -s 20` (with different system size)
`likwid-mpirun -np 4 -mpi intelmpi ../miniMD-ICC -s 20 -o 1 --yaml_screen` (with yaml output to stdout)


## Meggie
On meggie likwid-mpirun currently does not work.

* Request interactive job on Meggie with fixed frequency and likwid profiling:

* Load required modules:
`module load intel64`

* Run benchmark:
`cd ./data`
`mpirun -np 4 ../miniMD-ICC` (Other options same as on emmy)
