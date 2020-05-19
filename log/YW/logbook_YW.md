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
   1.1 TODO
   1.2 TODO
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
   * Fon:
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


<!-----------------------------------------------------------------------------
Describe in detail how to configure and setup the testcases(es)
------------------------------------------------------------------------------>
## Testcase description

<!-----------------------------------------------------------------------------
All steps required to run the testcase and control affinity for application
------------------------------------------------------------------------------>
## How to run software


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
## Result <NAME-TAG>-<ID>

### Problem: <DESCRIPTION>


### Measurement <NAME-TAG>-<ID>.1
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

![Example Graph](graph.png?raw=true "Example Meassurement")

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
