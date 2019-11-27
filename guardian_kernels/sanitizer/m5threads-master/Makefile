CC = arm-linux-gnueabi-gcc
CFLAGS = -O3 -static -march=armv7-a

.PHONY: all clean

all:  libpthread.a(pthread.o) pthread.o

clean:
	$(RM) *.o libpthread.a
