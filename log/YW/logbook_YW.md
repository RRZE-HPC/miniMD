<!-----------------------------------------------------------------------------
This document should be written based on the Github flavored markdown specs:
https://github.github.com/gfm/
It can be converted to html or pdf with pandoc:
pandoc -s -o logbook.html  -f gfm -t html logbook.md
pandoc test.txt -o test.pdf
or with the kramdown converter:
kramdown --template document  -i GFM  -o html logbook.md

If checked in as part of a github project html is automatically generated if
using the github web interface.

Optional: Document how much time was spent. A simple python command line tool
for time tracking is [Watson](http://tailordev.github.io/Watson/).
------------------------------------------------------------------------------>

<!-----------------------------------------------------------------------------
The Agenda section is a scratchpad area for planning and Todo list
------------------------------------------------------------------------------>
# Agenda
1. Scaling and performance measurement (handle MiniMD as "blackbox")
   1. Scaling within one socket
   2. Scaling within one node (two sockets)
   3. Scaling accross multiple nodes
2. Weak scaling
3. Strong & weak scaling (on one and many nodes) on Meggie
4. MPI analysis using ITAC
<!--
Example for referencing an image:

![Plot title](figures/example.png "ALT Text")
-->
<!-----------------------------------------------------------------------------
START BLOCK PREAMBLE -  Global information required in all steps: Add all
information required to build and benchmark the application. Should be extended
and maintained during the project.
------------------------------------------------------------------------------>
# Project Description

* Start date: 06/05/2020
* Ticket ID:
* Home HPC center:
* Contact HPC center:
   * Name: Yannick Wallerer
   * E-Mail: yannick.wallerer@fau.de

<!-----------------------------------------------------------------------------
Formulate a clear and specific performance target
------------------------------------------------------------------------------>
## Target
Performance Analysis of MiniMD with focus on MPI communication.


## How to build software
See README.md.  
Additional notes on the build process:  
**Used Compiler:**  
Intel C-Compiler (version 19.0.5.281)  
Compiler options: `-O3 -xAVX` 

## Hardware
All tests are done on one of the following clusters at RRZE:
### Emmy
* CPU name:	Intel(R) Xeon(R) CPU E5-2660 v2 @ 2.20GHz
* CPU type:	Intel Xeon IvyBridge EN/EP/EX processor
* If not stated otherwise, turbo mode was not used. The frequency is then fixed at 2.2 GHz
### Meggie
* CPU name:	Intel(R) Xeon(R) CPU E5-2630 v4 @ 2.20GHz
* CPU type:	Intel Xeon Broadwell EN/EP/EX processor
* If not stated otherwise, turbo mode was not used. The frequency is then fixed at 2.2 GHz

## Application Info

### Scaling
MiniMD applies strong scaling, i.e. the problem size does **not** change with the number of MPI processes. The problem size (number of atoms) can be adapted with the `-s` option (see below). Additionally, the number of timesteps can be changed in the input file (`data/in.lj.miniMD`).

### Relevant MiniMD options
* Full-neighbor variant: `--half_neigh 0` (preferably used over half-neighbor variant)
* Apply different problem sizes (number of atoms):
   * 4k   : `-s 10`
   * 16k  : `-s 16`
   * 32k  : `-s 20`
   * 108k : `-s 30`
   * 256k : `-s 40`
   * 864k : `-s 60`
* Alternatively to the options above, the problem size can be set manually in all spatial dimension using the options `-nx`, `-ny` and `-nz`
* Set the number of timesteps: `-n 1000` (here: 1000 timesteps)
* Level of yaml output: `-o / --yaml_output <int>`
* Write yaml output to stdout: `--yaml_screen`
* set input file to be used: `-i / --input_file <string>` (default: in.lj.miniMD)



## How to run software
On Emmy, the application can be run with likwid-mpirun from the data directory:
`$ likwid-mpirun [Likwid-Options] ../miniMD-ICC [MiniMD-Options]`

Relevant options for likwid-mpirun:
* `-np 4`: specify 4 MPI-Processes
* `-nperdomain S:10`: specify at most 10 MPI processes per socket

Alternatively, Intel mpirun can be used on both clusters. Relevant options are:
* `-np 100`: specify 100 MPI processes
* `-ppn 20`: specify at most 20 processes per node
* `-env I_MPI_PIN_PROCESSOR_LIST 0,1,2,3,4,5,6,7,8,9`: do explicit pinning (here, for each node, the first 10 cores are used)
* `-trace`: activate trace output, which can be analyzed by the intel traceanalyzer later

Typically, a series of scaling runs is done using shell scripts. Therefore, such an example shell script ("example_script_emmy.sh") for the use on emmy is also provided. It can be used as follows:
`$ qsub -l nodes=40:ppn=40:f2.2,walltime=01:00:00 example_script_emmy.sh`

<!-----------------------------------------------------------------------------
Test Documentation:
------------------------------------------------------------------------------>

## Testcases
### Testcase 1.1: Strong scaling on one node
Performance of MiniMD on one Node depending on the number of MPI processes:

![Test1.1.1](images/testcase_1_1_strong_single/speedup_strong_single.png?raw=true "Test1.1.1")  
<br/>

![Test1.1.2](images/testcase_1_1_strong_single/par_eff_strong_single.png?raw=true "Test1.1.2")
<br/>
As it can be seen from the graphs above, within one node, both emmy and meggie achieve almost identical speedup curves. However, this doesn't mean, that both machines achieve the same performance, which can be seen in the following graph:
<br/>
![Test1.1.3](images/testcase_1_1_strong_single/performance_strong_single.png?raw=true "Test1.1.3")
<br/>
We see, that Meggie performs better than Emmy although both worked at a frequency of 2.2 GHz and although for both runs the **same** binaries were used. Apparently, the program handling on an instruction level on Meggie is better than on Emmy. 

### Testcase 1.2: Scaling accross multiple nodes (Emmy)
 Each Node contains 20 cores. To make use the full power of each node, 20 MPI-processes were chosen as granularity in this measurement. In other words, for each measurement a set of n nodes was used to run n * 20 MPI processes. To make sure these processes are distributed equally, the likwid-mpirun option `-nperdomain S:10` can used. This way, each node (containing two sockets with 10 cores each) is assigned 10 MPI-Processes per socket. When working with Intel mpirun, the options `-ppn 20` and / or explicit process pinning should be used. 
In order to achieve more reliable results, each measurement has been done 3 times. Afterwards, the average has been formed over the distinct results.

**Used simulation settings:**  
* 10000  time steps

**Measurement:**

![Test1.2.1](images/testcase_1_2_strong_multi/comp_strong_multi.png?raw=true "Test1.2.1")
<br/>

![Test1.2.2](images/testcase_1_2_strong_multi/comp_strong_multi_par_eff.png?raw=true "Test1.2.2")
<br/>

In the graphs we see, that for scaling over multiple nodes, Meggie is capable of achieving higher speedup rates than Emmy. This can probably be explaind with the faster network used in Meggie.

### Testcase 2.1: Weak scaling on one node
In weak scaling, the problem size is increased linearly with the number of processes. As this isn't supported by MiniMD itself, shell scripts for the use on Emmy and Meggie are provided (see directory "weak_scaling_scripts").

Example usage on Emmy: `$ qsub -l nodes=1:ppn=40:f2.2,walltime=01:00:00 weak_single.sh` 
Example usage on Meggie: `$ sbatch weak_single_meggie.sh` (here, specifications about the used nodes are included in the shell script)

For the weak scaling runs, a basic problemsize of 16 x 16 x 16 was used with 10000 timesteps:

![Test2.1.1](images/testcase_2_1_weak_single/weak_single.png?raw=true "Test2.1.1")  
<br/>

![Test2.1.2](images/testcase_2_1_weak_single/weak_single_thread.png?raw=true "Test2.1.2")

### Testcase 2.2: Weak scaling on multiple nodes

Weak scaling over multiple nodes (using 20 MPI processes per node):

![Test2.2.1](images/testcase_2_2_weak_multi/weak_multi.png?raw=true "Test2.2.1")  
<br/>

![Test2.2.2](images/testcase_2_2_weak_multi/weak_multi_thread.png?raw=true "Test2.2.2")

### Testcase 3: Further MPI analysis using ITAC
Depending on the number of processes, MiniMD performs 1D, 2D or 3D domain decompostion according to the following strategy:
* Iterate over all possible domain decompositions
* For each potential decomposition, calculate the surface of the subdomains (reason: a smaller subdomain surface leads to a smaller data volume which must be communicated between the processes in each timestep)
* Accept the decomposition with the smallest surface

In general, a higher dimensional decomposition leads to a smaller subdomain surface. Therefore, 3D decomposition is used preferably. 1D decomposition is only used if both other decomposition types fail (e.g. for prime numbers).  

Example for 1D decomposition (using 19 processes):

![Test3.1.1](images/testcase_3_itac/plot_message_19proc_4k.png?raw=true "Test3.1.1")  
<br/>

Example for 2D decomposition (using 20 processes): 

![Test3.1.1](images/testcase_3_itac/plot_message_20proc_4k.png?raw=true "Test3.1.1")  
<br/>



<!--
## Effort

* Time spent:
-->

<!-----------------------------------------------------------------------------
END BLOCK ANALYST
------------------------------------------------------------------------------>

<!-----------------------------------------------------------------------------
START BLOCK SUMMARY - This block is only required if multiple analysts worked
on the project.
------------------------------------------------------------------------------>
# Overall Summary

* End date: DD/MM/YYYY

<!--
## Total Effort

* Total time spent:
* Estimated core hours saved:
-->
<!-----------------------------------------------------------------------------
END BLOCK SUMMARY
------------------------------------------------------------------------------>
