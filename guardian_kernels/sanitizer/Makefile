INCLUDES = -I.
CFLAGS = -Wall -Wextra -Werror -O3  -DUSE_PUBLIC_MALLOC_WRAPPERS -DGEM5 -march=armv7-a $(INCLUDES)
CPPFLAGS = $(CFLAGS)

all: libraries obj

bin obj lib:
	mkdir -p $@

libraries: lib/libmalloc.a

lib/lib%.a: obj/%.c.o | lib
	$(AR) rcs $@ $^

obj/%.c.o: %.c | obj
	arm-linux-gnueabi-gcc  -o $@ -c $^ $(CFLAGS)

clean:
	@rm -rf *~ bin lib obj
