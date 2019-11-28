if [ "$#" -lt 2 ]; then
    echo "Correct arguments: bench args (stdin)" && exit 1
fi
for cores in 12 8 6 4 2 1
do
echo "$cores" > secmap.ini
echo "-00000000" >> secmap.ini
  $BASE/gem5-guardian/build/ARM/gem5.opt $BASE/gem5-guardian/configs/example/se.py  -c "$1;$(printf $BASE'/guardian_kernels/performance_counter;%.0s' {1..12})" -o "$2" -i "$3" --l2cache -n $(($cores + 1))  --num-main-cores=1 --cpu-type=arm_detailed --cpu2-type=minor --caches --l2cache --maxinsts=1000000000 --fast-forward=1000000000 --mem-size=2048MB; mv m5out/stats.txt m5out/statscounter$cores.txt; mv m5out/delays.txt m5out/delayscounter$cores.txt
done
echo "" > secmap.ini
  $BASE/gem5-guardian/build/ARM/gem5.opt $BASE/gem5-guardian/configs/example/se.py  -c "$1" -o "$2" -i "$3" --l2cache -n 1  --num-main-cores=1 --cpu-type=arm_detailed --cpu2-type=minor --caches --l2cache --maxinsts=1000000000 --fast-forward=1000000000 --mem-size=2048MB; mv m5out/stats.txt m5out/statscounterno.txt
