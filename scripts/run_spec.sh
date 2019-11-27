cd ..
export BASE=$(pwd)
cd SPEC/benchspec/CPU2006/
N=$(grep ^cpu\\scores /proc/cpuinfo | uniq |  awk '{print $4}')
M=$(grep MemTotal /proc/meminfo | awk '{print $2}')
G=$(expr $M / 2097152)
P=$((G<N ? G : N))
for bench in astar bwaves bzip2 cactusADM calculix gcc GemsFDTD gobmk h264ref lbm leslie3d mcf milc namd omnetpp povray sjeng tonto wrf xalancbmk
do
  ((i=i%P)); ((i++==0)) && wait
  (
  IN=$(grep $bench $BASE/spec_confs/input.txt | awk -F':' '{print $2}')
  BOUNDS=$(grep $bench $BASE/spec_confs/bounds.txt | awk -F':' '{print $2}')
  BIN=$(grep $bench $BASE/spec_confs/binaries.txt | awk -F':' '{print $2}')
  ARGS=$(grep $bench $BASE/spec_confs/args.txt | awk -F':' '{print $2}')
  cd *$bench/run/run_base_ref_aarch64.0000
  $BASE/scripts/gem5_scripts/run_cfi.sh $BIN.aarch64 "$ARGS" "$IN" "$BOUNDS"
  $BASE/scripts/gem5_scripts/run_counter.sh $BIN.aarch64 "$ARGS" "$IN"
  $BASE/scripts/gem5_scripts/run_rowhammer.sh $BIN.aarch64 "$ARGS" "$IN"
  $BASE/scripts/gem5_scripts/run_shadow.sh $BIN.aarch64 "$ARGS" "$IN"
  cd ../run_base_ref_armdlmalloc.0000
  $BASE/scripts/gem5_scripts/run_sanitizer.sh $BIN.armdlmalloc "$ARGS" "$IN") &
done
cd $BASE/scripts
