if [ "$#" -lt 2 ]; then
    echo "Correct arguments: bench args (stdin)" && exit 1
fi
for cores in 12 8 6 4 2 1
do
echo "$cores" > secmap.ini
echo "--0000000" >> secmap.ini
  $BASE/gem5-guardian/build/ARM/gem5.opt $BASE/gem5-guardian/configs/example/se.py  -c "$1" -o "$2" -i "$3" --l2cache -n 13  --num-main-cores=1 --cpu-type=arm_detailed --cpu2-type=minor --caches --l2cache  --mem-size=2048MB; mv m5out/stats.txt m5out/statssanitizer$cores.txt; mv m5out/delays.txt m5out/delayssanitizer$cores.txt
done
echo "12" > secmap.ini
echo "--0000000" >> secmap.ini
  $BASE/gem5-guardian/build/ARM/gem5.opt $BASE/gem5-guardian/configs/example/se2ghz.py  -c "$1" -o "$2" -i "$3" --l2cache -n 13  --num-main-cores=1 --cpu-type=arm_detailed --cpu2-type=minor --caches --l2cache  --mem-size=2048MB; mv m5out/stats.txt m5out/statssanitizer24.txt;mv m5out/delays.txt m5out/delayssanitizer24.txt;
echo "" > secmap.ini
  $BASE/gem5-guardian/build/ARM/gem5.opt $BASE/gem5-guardian/configs/example/se.py  -c "$1" -o "$2" -i "$3" --l2cache -n 13  --num-main-cores=1 --cpu-type=arm_detailed --cpu2-type=minor --caches --l2cache  --mem-size=2048MB; mv m5out/stats.txt m5out/statssanitizerno.txt
