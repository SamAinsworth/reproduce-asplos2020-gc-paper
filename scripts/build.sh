cd ../guardian_kernels/sanitizer
./build.sh
cd m5threads-master
#make
cd ../..
aarch64-linux-gnu-gcc -O3 -static controlflow.c -o controlflow
aarch64-linux-gnu-gcc -O3 -static controlflow_fine.c -o controlflow_fine
aarch64-linux-gnu-gcc -O3 -static performance_counter.c -o performance_counter
aarch64-linux-gnu-gcc -O3 -static rowhammer.c -o rowhammer
aarch64-linux-gnu-g++ -O3 -static shadowstackpar.cpp -o shadowstackpar
aarch64-linux-gnu-g++ -O3 -static shadowstackparfilter.cpp -o shadowstackparfilter
cd ../gem5-guardian
scons -j4 build/ARM/gem5.opt
cd ../scripts
