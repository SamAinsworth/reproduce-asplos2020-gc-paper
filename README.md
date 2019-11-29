Artefact Evaluation for "The Guardian Council: Parallel Programmable Hardware Security", ASPLOS 2020. 
==================================================

This repository contains artefacts and workflows 
to reproduce experiments from the ASPLOS 2020 paper 
by S. Ainsworth and T. M. Jones

"The Guardian Council: Parallel Programmable Hardware Security"

Hardware pre-requisities
========================
* An x86-64 system (more cores will improve simulation time for the full workflow).

Software pre-requisites
=======================

* Linux operating system (We used Ubuntu 16.04 and Ubuntu 18.04)
* A SPEC CPU2006 iso, placed in the root directory of the repository (We used v1.0), for the full workflow, though a short evaluation can be completed without.

Installation and Building
========================

You can install this repository as follows:

```
git clone https://github.com/SamAinsworth/reproduce-asplos2020-gc-paper
```

All scripts from here onwards are assumed to be run from the scripts directory, from the root of the repository:

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

To compile SPEC CPU2006 for the Guardian Council (only needed for a full evaluation), first place your SPEC .iso file (other images can be used by modifying the build_spec.sh script first) in the root directory of the repository (next to the file 'PLACE_SPEC_ISO_HERE'). Then, from the scripts directory, run

```
./build_spec.sh
```

Once this has successfully completed, it will build and set up run directories for all of the benchmarks (the runs themselves will fail, as the binaries are cross compiled).




Running experimental workflows (reproducing figures)
====================================================

For a quick evaluation, once everything bar SPEC is built, from the scripts file run
```
./run_bitcount.sh
```

This will run a short, but representative open source workload which will induce a significant amount of compute from the Guardian Kernels, and can be completed in around 2 hours.


For the full evaluation, with the SPEC CPU2006 workloads from the paper, run

```
./run_spec.sh
```

to resimulate the Guardian Council's experiments. This will take several days, depending on the amount of RAM and number of cores on your system. By default, the script will run as many workloads in parallel as you have physical cores, as long as you have enough RAM to do so. To change this default, alter the value of 'P' inside run_spec.sh.

If any unexpected behaviour is observed, please report it to the authors.

Validation of results
====================================================

To generate graphs of the data, from the scripts folder run

```
./plot_bitcount.sh
```

or for the full evaluation:

```
./plot_spec.sh
```

This will extract the data from the simulation runs' m5out/stats.txt files, and plot it using gnuplot. The plots themselves will be in the folder plots, and the data covered should look broadly similar to the plots for figures 4 and 7 from the paper.  For the small evaluation, bitcount was not included in the original paper, and so we have provided sample data and results in the folder sample_plots.

The raw data will be accessible in the run directories within the spec or bitcount folder, as stats*.txt and delays*.txt.


If anything is unclear, or any unexpected results occur, please report it to the authors.

Authors
=======
S. Ainsworth and T. M. Jones

Acknowledgements
===============
This work was supported by the Engineering and Physical Sciences Research Council (EPSRC), through grant references EP/K026399/1, EP/P020011/1 and EP/M506485/1, and ARM Ltd.

Customisation 
===============

* Workloads: New workloads can be run with each of the Guardian Kernels provided in the artefact. An example of how such a workload should be compiled is given in the Makefile of the bitcount directory. To run a workload on the gem5-guardian simulator, use the scripts in scripts/gem5_scripts, after exporting the BASE variable:

```
export BASE=*YOUR_ARTEFACT_ROOT*
```

The *nofwd variants can be used to run full short workloads. The others are used to fast forward and sample (as is necessary for longer workloads such as SPEC).

Workloads should be for Aarch64 or Aarch32, and statically linked, to run on the simulator. For shared-memory Guardian Kernels (Sanitizer in our existing set), you must compile the allocator into the binary: see bitcount's Makefile for more information.

* Guardian Kernels: You can create new Guardian Kernels to evaluate on the simulator. These are written as standard C/C++ programs, with custom instructions (typically implemented as inline ASM, but can be imported from the m5ops list - see the example kernels for more information) for FIFO queues and setup. The Filter and Mapper are programmed with secmap.ini files in the root of your simulation run directory. An example of how this is programmed is given in guardian_kernels/example_filter_map.ini. Multiple kernels can be run simultaneously by adding further lines to secmap.ini.

* Simulator: Much of the code for the Guardian Council is implemented in gem5-guardian/src/mem/cache/securelogentry.hh and .cc. The commit path of the O3CPU (src/cpu/o3/commit_impl.hh) can be altered to add further observation channels. If you would like further information on modifying the simulator, please contact the authors.

Troubleshooting
===============

* We had an issue on recent Linux kernels with compiling m5threads (for the Sanitizer guardian kernel). To this end, the make command is commented out in our buildscript, and the object file for m5threads preshipped. If this causes issues, please try to rebuild m5threads (guardian_kernels/sanitizer/m5threads-master) and if that doesn't solve the issue report it to the authors.

* You may have to specify a toolset when SPEC is being built. We chose linux-suse101-AMD64.

* The gnuplot scripts will issue several warnings for the small evaluation - these can be ignored, and are caused by using the same scripts as the full evaluation.

