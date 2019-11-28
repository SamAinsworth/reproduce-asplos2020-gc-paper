if [ "$#" -lt 2 ]; then
    echo "Correct arguments: bench args (stdin)" && exit 1
fi
for cores in 11 7 5 3 1
do
echo "1" > secmap.ini
echo "000000000" >> secmap.ini
echo "$cores" >> secmap.ini
echo "000||0000" >> secmap.ini
  $BASE/gem5-guardian/build/ARM/gem5.opt $BASE/gem5-guardian/configs/example/se.py  -c "$1;$BASE/guardian_kernels/shadowstackpar;$BASE/guardian_kernels/shadowstackparfilter;$BASE/guardian_kernels/shadowstackparfilter;$BASE/guardian_kernels/shadowstackparfilter;$BASE/guardian_kernels/shadowstackparfilter;$BASE/guardian_kernels/shadowstackparfilter;$BASE/guardian_kernels/shadowstackparfilter;$BASE/guardian_kernels/shadowstackparfilter;$BASE/guardian_kernels/shadowstackparfilter;$BASE/guardian_kernels/shadowstackparfilter;$BASE/guardian_kernels/shadowstackparfilter;$BASE/guardian_kernels/shadowstackparfilter" -o "$2" -i "$3" --l2cache -n $(($cores + 2))  --num-main-cores=1 --cpu-type=arm_detailed --cpu2-type=minor --caches --l2cache --mem-size=2048MB; mv m5out/stats.txt m5out/statsshadow$cores.txt; mv m5out/delays.txt m5out/delaysshadow$cores.txt
done
echo "" > secmap.ini
  $BASE/gem5-guardian/build/ARM/gem5.opt $BASE/gem5-guardian/configs/example/se.py  -c "$1" -o "$2" -i "$3" --l2cache -n 1  --num-main-cores=1 --cpu-type=arm_detailed --cpu2-type=minor --caches --l2cache --mem-size=2048MB; mv m5out/stats.txt m5out/statsshadowno.txt
