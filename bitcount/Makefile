FILE = bitcnt_1.c bitcnt_2.c bitcnt_3.c bitcnt_4.c bitcnts.c bitfiles.c bitstrng.c bstr_i.c 

bitcnts: ${FILE} Makefile
	aarch64-linux-gnu-gcc-5 -static ${FILE} -O3 -o bitcnts
	arm-linux-gnueabi-gcc-5 -static ${FILE} -Wl,--allow-multiple-definition  -DUSE_PUBLIC_MALLOC_WRAPPERS  -DGEM5 -O3 -march=armv7-a ../guardian_kernels/sanitizer/malloc.o ../guardian_kernels/sanitizer/m5threads-master/libpthread.a -o bitcnts_a32

clean:
	rm -rf bitcnts output*
