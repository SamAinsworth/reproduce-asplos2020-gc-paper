cd ..
mkdir specmnt
mkdir SPEC
sudo mount -o loop *.iso specmnt
cd specmnt
./install.sh -d ../SPEC
cd ..
sudo umount specmnt
rm -r specmnt
cp spec_confs/aarch64.cfg SPEC/config
cp spec_confs/arm-gem5-dlmalloc.cfg SPEC/config
cd SPEC
. ./shrc   
runspec --config=aarch64.cfg --action=run --size=ref all -I --iterations=1
runspec --config=arm-gem5-dlmalloc.cfg --action=run --size=ref all -I --iterations=1
cd $BASE/scripts