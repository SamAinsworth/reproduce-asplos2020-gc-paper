cd ..
BASE=$(pwd)
mkdir specmnt
mkdir SPEC
sudo mount -o loop *.iso specmnt
cd specmnt
./install.sh -d ../SPEC
cd ..
sudo umount specmnt
rm -r specmnt
cp spec_confs/aarch64.cfg SPEC/config
cat <(echo "BASE= $BASE") spec_confs/arm-gem5-dlmalloc.cfg > SPEC/config/arm-gem5-dlmalloc.cfg 
cd SPEC
. ./shrc   
runspec --config=aarch64.cfg --action=build all -I
runspec --config=arm-gem5-dlmalloc.cfg --action=build all -I 
runspec --config=aarch64.cfg --action=run --size=ref all --iterations=1  -I
runspec --config=arm-gem5-dlmalloc.cfg --action=run --size=ref all --iterations=1  -I
cd $BASE/scripts
