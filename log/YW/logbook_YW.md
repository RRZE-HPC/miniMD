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
1. Scaling and performance meassurement (handle MiniMD as "blackbox")
   1. Scaling within one socket
   2. Scaling within one node (two sockets)
   3. Scaling accross multiple nodes
2. Code analysis of the MPI communication
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

<!--
## Customer Info
-->
<!--
* Name: <CUSTOMERNAME>
* E-Mail: john.doe@foo.bar
* Fon: <PHONENUMBER>
* Web: <URL>
-->

<!-----------------------------------------------------------------------------
All steps required to build the software including dependencies
------------------------------------------------------------------------------>
## How to build software
See README.md.


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
<!--
* Name: <APPLICATIONNAME>
* Domain: <APPLICATIONDOMAIN>
* Version: <VERSION>
-->


## How to run software
On Emmy, the application can be run with likwid-mpirun from the data directory:
`$ likwid-mpirun [Likwid-Options] ../miniMD-ICC [MiniMD-Options]`

Relevant options for likwid-mpirun:
* `-np 4`: specify 4 MPI-Processes
* `-nperdomain S:10`: specify at most 10 MPI processes per socket

<!-----------------------------------------------------------------------------
END BLOCK PREAMBLE
------------------------------------------------------------------------------>

<!-----------------------------------------------------------------------------
START BLOCK ANALYST - This block is required for any new analyst taking over
the project
------------------------------------------------------------------------------>

<!--
# Transfer to Analyst: <NAME-TAG>
-->
<!--
* Start date: DD/MM/YYYY
* Contact HPC center:
   * Name:
   * Fon:
   * E-Mail:
-->
<!-----------------------------------------------------------------------------
###############################################################################
START BLOCK BENCHMARKING - Run helper script machine-state.sh and store results
in directory session-<ID> named <hostname>.txt. Document everything that you
consider to be relevant for performance.
###############################################################################
------------------------------------------------------------------------------>

<!--
## Benchmarking <NAME-TAG>

### Testsystem
-->
<!--
* Host/Clustername:
* Cluster Info URL:
* CPU type:
* Memory capacity:
* Number of cores per node:
* Interconnect:
-->
<!--
### Software Environment
-->
<!--
**Compiler**:
* Vendor:
* Version:

**Libraries**:
* <LIBRARYNAME>:
   * Version:

**OS**:
* Distribution:
* Version:
* Kernel version:
-->
<!-----------------------------------------------------------------------------
Create a runtime profile. Which tool was used? How was the profile created.
Describe and discuss the runtime profile.
------------------------------------------------------------------------------>
<!--
## Runtime Profile <NAME-TAG>-<ID>
-->

<!-----------------------------------------------------------------------------
Perform a static code review.
------------------------------------------------------------------------------>
<!--
## Code review <NAME-TAG>-<ID>
-->

<!-----------------------------------------------------------------------------
Application benchmarking runs. What experiment was done? Add results or
reference plots in directory session-<NAME-TAG>-<ID>. Number all sections
consecutivley such that every section has a unique ID.
------------------------------------------------------------------------------>

## Testcases
### Testcase 1.1: Scaling on one node
Performance of MiniMD on one Node (of Emmy) depending on the number of MPI processes:

![Test1](test_1_1.png?raw=true "Test1")

An observation of the utilization of the different cores (using the tool `top`) of the node showed, that `likwid-mpirun` assigns the n MPI processes on the first n CPUs of the node (e.g. when 4 MPI processes are specified, CPU 0 - 3 are used).
As expected, the application scales well until 20 MPI processes. With more MPI processes, the performance decreases as then one or more CPUs must then execute more than one process.

### Testcase 1.2: Scaling accross multiple nodes
Performance of MiniMD using multiple nodes on Emmy. Each Node contains 20 cores. To make use the full power of each node, 20 MPI-processes were chosen as granularity in this meassurement. In other words, for each meassurement a set of n nodes was used to run n * 20 MPI processes. To make sure these processes are distributed equally, the likwid-mpirun option `-nperdomain S:10` was used. This way, each node (containing two sockets with 10 cores each) is assigned 10 MPI-Processes per socket.  
In order to achieve more reliable results, each meassurement has been done 3 times. Afterwards, the average has been formed over the distinct results.

**Used simulation settings:** 
* 131072 atoms (default if no -s option is used)  
* 10000  time steps

**Meassurement:**

![Speedup](default_10000steps_speedup.png?raw=true "Speedup")<br/>
![Performance](default_10000steps_total_performance.png?raw=true "Performance")<br/>
![Thread Performance](default_10000steps_thread_performance.png?raw=true "Thread Performance")

<!--
## Result <NAME-TAG>-<ID>
### Problem: <DESCRIPTION>
### Measurement <NAME-TAG>-<ID>.1
-->
<!--
Example for table:

| NP | runtime |
|----|---------|
| 1  | 2558.89 |
| 2  | 1425.20 |
| 4  | 741.97  |
| 8  | 449.23  |
| 10 | 371.39  |
| 20 | 233.90  |

```
Verbatim Text
```
-->


<!-----------------------------------------------------------------------------
Document the initial performance which serves as baseline for further progress
and is used to compute the achieved speedup. Document exactly how the baseline
was created.
------------------------------------------------------------------------------>
<!--
## Baseline
-->
<!--
* Time to solution:
* Performance:
-->

<!-----------------------------------------------------------------------------
Explain which tool was used and how the measurements were done. Store and
reference the results. If applicable discuss and explain profiles.
------------------------------------------------------------------------------>
<!--
## Performance Profile <NAME-TAG>-<ID>.2
-->

<!-----------------------------------------------------------------------------
Analysis and insights extracted from benchmarking results. Planning of more
benchmarks.
------------------------------------------------------------------------------>
<!--
## Analysis <NAME-TAG>-<ID>.3
-->


<!-----------------------------------------------------------------------------
Document all changes with  filepath:linenumber and explanation what was changed
and why. Create patch if applicable and store patch in referenced file.
------------------------------------------------------------------------------>
<!--
## Optimisation <NAME-TAG>-<ID>.4: <DESCRIPTION>
-->


<!-----------------------------------------------------------------------------
###############################################################################
END BLOCK BENCHMARKING
###############################################################################
------------------------------------------------------------------------------>

<!-----------------------------------------------------------------------------
Wrap up the final result and discuss the speedup.
Optional: Document how much time was spent. A simple python command line tool
for time tracking is [Watson](http://tailordev.github.io/Watson/).
------------------------------------------------------------------------------>
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
