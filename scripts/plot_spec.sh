cd ..
BASE=$(pwd)
rm $BASE/plots/*.data
for bench in astar bwaves bzip2 cactusADM calculix gcc GemsFDTD gobmk h264ref lbm leslie3d mcf milc namd omnetpp povray sjeng tonto wrf xalancbmk
do
echo $bench >> $BASE/plots/counter.data
echo $bench >> $BASE/plots/counterdelays.data
echo $bench >> $BASE/plots/integrity.data
echo $bench >> $BASE/plots/integritydelays.data
echo $bench >> $BASE/plots/integrityfine.data
echo $bench >> $BASE/plots/integrityfinedelays.data
echo $bench >> $BASE/plots/rowhammer.data
echo $bench >> $BASE/plots/rowhammerdelays.data
echo $bench >> $BASE/plots/sanitizer.data
echo $bench >> $BASE/plots/sanitizerdelays.data
echo $bench >> $BASE/plots/shadow.data
echo $bench >> $BASE/plots/shadowdelays.data
base=$(grep sim_se $BASE/SPEC/benchspec/CPU2006/*$bench/run/run_base_ref_aarch64.0000/m5out/statsshadowno.txt | awk '{print $2}')
for n in 1 3 5 7 11
do
slow=$(grep sim_se $BASE/SPEC/benchspec/CPU2006/*$bench/run/run_base_ref_aarch64.0000/m5out/statsshadow$n.txt  | awk '{print $2}')
echo $n $(echo "scale=3;$slow / $base" | bc -l) >> $BASE/plots/shadow.data
echo "" >> $BASE/plots/shadow.data
done
base=$(grep sim_se $BASE/SPEC/benchspec/CPU2006/*$bench/run/run_base_ref_aarch64.0000/m5out/statscounterno.txt | awk '{print $2}')
for n in 1 2 4 6 8 12
do
slow=$(grep sim_se $BASE/SPEC/benchspec/CPU2006/*$bench/run/run_base_ref_aarch64.0000/m5out/statscounter$n.txt  | awk '{print $2}')
echo $n $(echo "scale=3;$slow / $base" | bc -l) >> $BASE/plots/counter.data
echo "" >> $BASE/plots/counter.data
done
base=$(grep sim_se $BASE/SPEC/benchspec/CPU2006/*$bench/run/run_base_ref_aarch64.0000/m5out/statsrowhammerno.txt | awk '{print $2}')
for n in 1 2 4 6 8 12
do
slow=$(grep sim_se $BASE/SPEC/benchspec/CPU2006/*$bench/run/run_base_ref_aarch64.0000/m5out/statsrowhammer$n.txt  | awk '{print $2}')
echo $n $(echo "scale=3;$slow / $base" | bc -l) >> $BASE/plots/rowhammer.data
echo "" >> $BASE/plots/rowhammer.data
done
base=$(grep sim_se $BASE/SPEC/benchspec/CPU2006/*$bench/run/run_base_ref_aarch64.0000/m5out/statsstatscfino.txt | awk '{print $2}')
for n in 1 2 4 6 8 12
do
slow=$(grep sim_se $BASE/SPEC/benchspec/CPU2006/*$bench/run/run_base_ref_aarch64.0000/m5out/statsstatscfifine$n.txt  | awk '{print $2}')
echo $n $(echo "scale=3;$slow / $base" | bc -l) >> $BASE/plots/integrityfine.data
echo "" >> $BASE/plots/integrityfine.data
slow=$(grep sim_se $BASE/SPEC/benchspec/CPU2006/*$bench/run/run_base_ref_aarch64.0000/m5out/statsstatscfi$n.txt  | awk '{print $2}')
echo $n $(echo "scale=3;$slow / $base" | bc -l) >> $BASE/plots/integrity.data
echo "" >> $BASE/plots/integrity.data
done
base=$(grep sim_se $BASE/SPEC/benchspec/CPU2006/*$bench/run/run_base_ref_armdlmalloc.0000/m5out/statssanitizerno.txt | awk '{print $2}')
for n in 1 2 4 6 8 12 24
do
slow=$(grep sim_se $BASE/SPEC/benchspec/CPU2006/*$bench/run/run_base_ref_armdlmalloc.0000/m5out/statssanitizer$n.txt  | awk '{print $2}')
echo $n $(echo "scale=3;$slow / $base" | bc -l) >> $BASE/plots/sanitizer.data
echo "" >> $BASE/plots/sanitizer.data
done
echo "" >> $BASE/plots/counter.data
echo "" >> $BASE/plots/integrity.data
echo "" >> $BASE/plots/integrityfine.data
echo "" >> $BASE/plots/rowhammer.data
echo "" >> $BASE/plots/sanitizer.data
echo "" >> $BASE/plots/shadow.data
cat $BASE/SPEC/benchspec/CPU2006/*$bench/run/run_base_ref_aarch64.0000/m5out/delaysshadow7.txt >> $BASE/plots/shadowdelays.data
cat $BASE/SPEC/benchspec/CPU2006/*$bench/run/run_base_ref_aarch64.0000/m5out/delayscounter8.txt >> $BASE/plots/counterdelays.data
cat $BASE/SPEC/benchspec/CPU2006/*$bench/run/run_base_ref_aarch64.0000/m5out/delayscfi8.txt >> $BASE/plots/integritydelays.data
cat $BASE/SPEC/benchspec/CPU2006/*$bench/run/run_base_ref_aarch64.0000/m5out/delayscfifine8.txt >> $BASE/plots/integrityfinedelays.data
cat $BASE/SPEC/benchspec/CPU2006/*$bench/run/run_base_ref_aarch64.0000/delaysrowhammer8.txt >> $BASE/plots/rowhammerdelays.data
cat $BASE/SPEC/benchspec/CPU2006/*$bench/run/run_base_ref_armdlmalloc.0000/m5out/delayssanitizer8.txt >> $BASE/plots/sanitizerdelays.data
echo "" >> $BASE/plots/counterdelays.data
echo "" >> $BASE/plots/integritydelays.data
echo "" >> $BASE/plots/integrityfinedelays.data
echo "" >> $BASE/plots/rowhammerdelays.data
echo "" >> $BASE/plots/sanitizerdelays.data
echo "" >> $BASE/plots/shadowdelays.data
done
cd $BASE/plots/
gnuplot counter.gp
gnuplot integrity.gp
gnuplot integrityfine.gp
gnuplot rowhammer.gp
gnuplot sanitizer.gp
gnuplot shadow.gp
gnuplot counterdelays.gp
gnuplot shadowdelays.gp
gnuplot integritydelays.gp
gnuplot integrityfinedelays.gp
gnuplot rowhammerdelays.gp
gnuplot sanitizerdelays.gp
cd $BASE/scripts/
