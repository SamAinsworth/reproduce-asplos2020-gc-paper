cd ..
set -u
export BASE=$(pwd)
cd SPEC/benchspec/CPU2006/
N=$(grep ^cpu\\scores /proc/cpuinfo | uniq |  awk '{print $4}')
M=$(grep MemTotal /proc/meminfo | awk '{print $2}')
G=$(expr $M / 2097152)
P=$((G<N ? G : N))
i=0
for bench in astar bwaves bzip2 cactusADM calculix gcc GemsFDTD gobmk h264ref lbm leslie3d mcf milc namd omnetpp povray sjeng tonto wrf xalancbmk
do
  ((i=i%P)); ((i++==0)) && wait
  (
  IN=$(grep $bench $BASE/spec_confs/input.txt | awk -F':' '{print $2}'| xargs)
  BIN=$(grep $bench $BASE/spec_confs/binaries.txt | awk -F':' '{print $2}' | xargs)
  BINA=./$(echo $BIN)"_base.aarch64"
  BINB=./$(echo $BIN)"_base.armdlmalloc"
  echo $BINA
  ARGS=$(grep $bench $BASE/spec_confs/args.txt | awk -F':' '{print $2}'| xargs)
  cd *$bench/run/run_base_ref_aarch64.0000
  $BASE/scripts/gem5_scripts/run_cfi.sh "$BINA" "$ARGS" "$IN" 
  $BASE/scripts/gem5_scripts/run_counter.sh "$BINA" "$ARGS" "$IN"
  $BASE/scripts/gem5_scripts/run_rowhammer.sh "$BINA" "$ARGS" "$IN"
  $BASE/scripts/gem5_scripts/run_shadow.sh "$BINA" "$ARGS" "$IN"
  cd ../run_base_ref_armdlmalloc.0000
  $BASE/scripts/gem5_scripts/run_sanitizer.sh "$BINB" "$ARGS" "$IN") & 
done
cd $BASE/scripts
