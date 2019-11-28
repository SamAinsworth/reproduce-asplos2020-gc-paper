cd ..
BASE=$(pwd)
rm $BASE/plots/*.data
for bench in bitcount
do
echo $bench >> $BASE/plots/counter.data
echo $bench >> $BASE/plots/integrity.data
echo $bench >> $BASE/plots/integrityfine.data
echo $bench >> $BASE/plots/rowhammer.data
echo $bench >> $BASE/plots/sanitizer.data
echo $bench >> $BASE/plots/shadow.data
base=$(grep sim_se $BASE/bitcount/m5out/statsshadowno.txt | awk '{print $2}')
for n in 1 3 5 7 11
do
slow=$(grep sim_se $BASE/bitcount/m5out/statsshadow$n.txt  | awk '{print $2}')
echo $n $(expr $slow / $base) >> $BASE/plots/shadow.data
echo "\n" >> $BASE/plots/shadow.data
done
base=$(grep sim_se $BASE/bitcount/m5out/statscounterno.txt | awk '{print $2}')
for n in 1 2 4 6 8 12
do
slow=$(grep sim_se $BASE/bitcount/m5out/statscounter$n.txt  | awk '{print $2}')
echo $n $(expr $slow / $base) >> $BASE/plots/counter.data
echo "\n" >> $BASE/plots/counter.data
done
base=$(grep sim_se $BASE/bitcount/m5out/statsrowhammerno.txt | awk '{print $2}')
for n in 1 2 4 6 8 12
do
slow=$(grep sim_se $BASE/bitcount/m5out/statsrowhammer$n.txt  | awk '{print $2}')
echo $n $(expr $slow / $base) >> $BASE/plots/rowhammer.data
echo "\n" >> $BASE/plots/rowhammer.data
done
base=$(grep sim_se $BASE/bitcount/m5out/statsstatscfino.txt | awk '{print $2}')
for n in 1 2 4 6 8 12
do
slow=$(grep sim_se $BASE/bitcount/m5out/statsstatscfifine$n.txt  | awk '{print $2}')
echo $n $(expr $slow / $base) >> $BASE/plots/integrityfine.data
echo "\n" >> $BASE/plots/integrityfine.data
slow=$(grep sim_se $BASE/bitcount/m5out/statsstatscfi$n.txt  | awk '{print $2}')
echo $n $(expr $slow / $base) >> $BASE/plots/integrity.data
echo "\n" >> $BASE/plots/integrity.data
done
base=$(grep sim_se *$bench/run/run_base_ref_armdlmalloc.0000/m5out/statssanitizerno.txt | awk '{print $2}')
for n in 1 2 4 6 8 12 24
do
slow=$(grep sim_se *$bench/run/run_base_ref_armdlmalloc.0000/m5out/statssanitizer$n.txt  | awk '{print $2}')
echo $n $(expr $slow / $base) >> $BASE/plots/sanitizer.data
echo "\n" >> $BASE/plots/sanitizer.data
done
echo "\n" >> $BASE/plots/counter.data
echo "\n" >> $BASE/plots/integrity.data
echo "\n" >> $BASE/plots/integrityfine.data
echo "\n" >> $BASE/plots/rowhammer.data
echo "\n" >> $BASE/plots/sanitizer.data
echo "\n" >> $BASE/plots/shadow.data
done
cd $BASE/plots/
gnuplot counter.gp
gnuplot integrity.gp
gnuplot integrityfine.gp
gnuplot rowhammer.gp
gnuplot sanitizer.gp
gnuplot shadow.gp
cd $BASE/scripts/
