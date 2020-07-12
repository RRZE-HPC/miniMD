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
As the focus here lies on analyzing the MPI mechanism, no SIMD vectorization is applied.  
**Used Compiler:**  
Intel C-Compiler (version 19.0.5.281)  
Compiler options: `-O3 -no-vec` 

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
* Full-neighbor variant: `--half_neigh 1` (preferably used over half-neighbor variant)
* Apply different problem sizes (number of atoms):
   * 4k   : `-s 10`
   * 16k  : `-s 16`
   * 32k  : `-s 20`
   * 108k : `-s 30`
   * 256k : `-s 40`
   * 864k : `-s 60`
* Level of yaml output: `-o / --yaml_output <int>`
* Write yaml output to stdout: `--yaml_screen`
* set input file to be used: `-i / --input_file <string>` (default: in.lj.miniMD)



## How to run software
On Emmy, the application can be run with likwid-mpirun from the data directory:
`$ likwid-mpirun [Likwid-Options] ../miniMD-ICC [MiniMD-Options]`

Relevant options for likwid-mpirun:
* `-np 4`: specify 4 MPI-Processes
* `-nperdomain S:10`: specify at most 10 MPI processes per socket

<!-----------------------------------------------------------------------------
Test Documentation:
------------------------------------------------------------------------------>

## Testcases
### Testcase 1.1: Scaling on one node (Emmy)
Performance of MiniMD on one Node (of Emmy) depending on the number of MPI processes:

![Test1.1.1](images/testcase_1_1_single_node/speedup_single_node.png?raw=true "Test1.1.1")

An observation of the utilization of the different cores (using the tool `top`) of the node showed, that `likwid-mpirun` assigns the n MPI processes on the first n CPUs of the node (e.g. when 4 MPI processes are specified, CPU 0 - 3 are used).
Beyond 20 MPI processes, a remarkable performance loss can be observed (see also the corresponding drop in the parallel efficiency below). This is expected, as then one or more CPU cores have to execute more than one MPI process.

![Test1.1.2](images/testcase_1_1_single_node/par_eff_single_node.png?raw=true "Test1.1.2")

### Testcase 1.2: Scaling accross multiple nodes (Emmy)
Performance of MiniMD using multiple nodes on Emmy. Each Node contains 20 cores. To make use the full power of each node, 20 MPI-processes were chosen as granularity in this measurement. In other words, for each measurement a set of n nodes was used to run n * 20 MPI processes. To make sure these processes are distributed equally, the likwid-mpirun option `-nperdomain S:10` was used. This way, each node (containing two sockets with 10 cores each) is assigned 10 MPI-Processes per socket.  
In order to achieve more reliable results, each measurement has been done 3 times. Afterwards, the average has been formed over the distinct results.

**Used simulation settings:**  
* 10000  time steps

**Measurement:**

![Test1.2.1](images/testcase_1_2_many_node/speedup_multi_node.png?raw=true "Test1.2.1")
<br/>

![Test1.2.2](images/testcase_1_2_many_node/par_eff_multi_node.png?raw=true "Test1.2.2")


### Testcase 2.1: Weak scaling on one node (Emmy)
In weak scaling, the problem size is increased linearly with the number of processes:

![Test2.1.1](images/testcase_1_3_weak_scaling/problem_size_single.png?raw=true "Test2.1.1")
<br/>
This lead to the following performance measurement:  

![Test2.1.2](images/testcase_1_3_weak_scaling/performance_single_node.png?raw=true "Test2.1.2")
<br/>

![Test2.1.3](testcase_1_3_weak_scaling/performance_thread_single_node.png?raw=true "Test2.1.3")

### Testcase 2.2: Weak scaling on multiple nodes (Emmy)

Weak scaling over multiple nodes (using 20 MPI processes per node):
<br/>
![Test2.2.1](images/testcase_1_3_weak_scaling/performance_total_multiple_nodes.png?raw=true "Test2.2.1")
<br/>

![Test2.2.2](images/testcase_1_3_weak_scaling/performance_thread_multiple_nodes.png?raw=true "Test2.2.2")

### Testcase 3.1: Strong scaling on Meggie
### Testcase 3.1.1: Single node
![Test3.1.1](images/testcase_3_meggie/strong_scaling/speedup_single.png?raw=true "Test3.1.1")
<br/>
![Test3.1.1](images/testcase_3_meggie/strong_scaling/par_eff_single.png?raw=true "Test3.1.1")
<br/>

### Testcase 3.1.2: Multiple nodes

### Testcase 3.2: Weak scaling on Meggie
### Testcase 3.2.1: Single node
![Test3.2.1](images/testcase_3_meggie/weak_scaling/weak_single_perf.png?raw=true "Test3.2.1")
<br/>
![Test3.2.1](images/testcase_3_meggie/weak_scaling/weak_single_perf_thread.png?raw=true "Test3.2.1")
<br/>

### Testcase 3.2.2: Multiple nodes



## Summary

* Time to solution:
* Performance:
* Speedup:

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
