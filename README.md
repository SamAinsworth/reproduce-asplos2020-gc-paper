Artefact Evaluation for "The Guardian Council: Parallel Programmable Hardware Security", ASPLOS 2020. 
==================================================

This repository contains artefacts and workflows 
to reproduce experiments from the ASPLOS 2020 paper 
by S. Ainsworth and T. M. Jones

"Software Prefetching for Indirect Memory Accesses"

Hardware pre-requisities
========================
* An x86-64 system

Software pre-requisites
=======================

* Linux operating system (We used Ubuntu 16.04)
* A SPEC CPU2006 iso, placed in the root directory of the repository.

Installation and Building
========================

You can install this repository as follows:

```
git clone https://github.com/SamAinsworth/reproduce-asplos2020-gc-paper
```

All scripts from here onwards are assumed to be run from the scripts directory, from the room of the repository:
```
cd reproduce-asplos2020-gc-paper
cd scripts
```

To install software package dependencies, run

```
./dependencies.sh
```

Then, in the scripts folder, to compile the Guardian Council simulator and the Guardian Kernels, run
```
./build.sh
```

To compile SPEC CPU2006 for the Guardian Council, first place your SPEC .iso file (other images can be used by modifying the build_spec.sh script first) in the root directory of the repository (next to the file 'PLACE_SPEC_ISO_HERE'). Then, from the scripts directory, run

```
./build_spec.sh
```

Once this has successfully completed, it will build and set up run directories for all of the benchmarks (the runs themselves will fail, as the binaries are cross compiled).




Running experimental workflows (reproducing figures)
====================================================

Once everything is successfully built, from the scripts file run

```
./run_spec.sh
```

to resimulate the Guardian Council's experiments. This will take several hours to a day, depending on the amount of RAM and number of cores on your system. By default, the script will run as many workloads in parallel as you have physical cores, as long as you have enough RAM to do so. To change this default, alter the value of 'P' inside run_spec.sh.

If any unexpected behaviour is observed, please report it to the authors.

Validation of results
====================================================

To generate graphs of the data, from the scripts folder run

```
./plot_spec.sh
```

This will extract the data from the simulation runs' m5out/stats.txt files, and plot it using gnuplot. The plots themselves will be in the folder plots, and the data covered should look broadly similar to the plots for figures 4 and 7 from the paper.

The raw data will be accessible in the run directories within the spec folder, as stats*.txt and delays*.txt.


If anything in unclear, or any unexpected results occur, please report it to the authors.

Authors
=======
S. Ainsworth and T. M. Jones

Acknowledgments
===============
This work was supported by the Engineering and Physical Sciences Research Council (EPSRC), through grant references EP/K026399/1, EP/P020011/1 and EP/M506485/1, and ARM Ltd.


Customisation 
===============

TODO

