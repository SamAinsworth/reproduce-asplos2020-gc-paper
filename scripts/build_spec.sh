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
runspec --config=arm-gem5-dlmalloc.cfg --action=build astar bwaves bzip2 cactusADM calculix gcc GemsFDTD gobmk h264ref lbm leslie3d mcf milc namd omnetpp povray sjeng tonto wrf xalancbmk -I 
runspec --config=aarch64.cfg --action=build astar bwaves bzip2 cactusADM calculix gcc GemsFDTD gobmk h264ref lbm leslie3d mcf milc namd omnetpp povray sjeng tonto wrf xalancbmk -I
runspec --config=aarch64.cfg --action=run --size=ref astar bwaves bzip2 cactusADM calculix gcc GemsFDTD gobmk h264ref lbm leslie3d mcf milc namd omnetpp povray sjeng tonto wrf xalancbmk --noreportable --iterations=1  -I
runspec --config=arm-gem5-dlmalloc.cfg --action=run --size=ref astar bwaves bzip2 cactusADM calculix gcc GemsFDTD gobmk h264ref lbm leslie3d mcf milc namd omnetpp povray sjeng tonto wrf xalancbmk  --noreportable --iterations=1  -I
cd $BASE/scripts
