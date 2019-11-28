cd ../guardian_kernels/sanitizer
./build.sh
make
cd m5threads-master
#make
cd ../..
aarch64-linux-gnu-gcc -O3 -static controlflow.c ../gem5-guardian/util/m5/m5op_arm_A64.S -o controlflow
aarch64-linux-gnu-gcc -O3 -static controlflow_fine.c ../gem5-guardian/util/m5/m5op_arm_A64.S -o controlflow_fine
aarch64-linux-gnu-gcc -O3 -static performance_counter.c ../gem5-guardian/util/m5/m5op_arm_A64.S -o performance_counter
aarch64-linux-gnu-gcc -O3 -static rowhammer.c ../gem5-guardian/util/m5/m5op_arm_A64.S -o rowhammer
aarch64-linux-gnu-g++ -O3 -static shadowstackpar.cpp ../gem5-guardian/util/m5/m5op_arm_A64.S -o shadowstackpar
aarch64-linux-gnu-g++ -O3 -static shadowstackparfilter.cpp ../gem5-guardian/util/m5/m5op_arm_A64.S -o shadowstackparfilter
cd ../bitcount
make
cd ../gem5-guardian
scons -j4 build/ARM/gem5.opt
cd ../scripts
