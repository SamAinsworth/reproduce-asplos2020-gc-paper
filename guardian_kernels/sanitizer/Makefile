INCLUDES = -I.
CFLAGS = -Wall -Wextra -Werror -O3 $(INCLUDES)
CPPFLAGS = $(CFLAGS)

all: libraries obj

bin obj lib:
	mkdir -p $@

libraries: lib/libmalloc.a

lib/lib%.a: obj/%.c.o | lib
	$(AR) rcs $@ $^

obj/%.c.o: %.c | obj
	arm-linux-gnueabi-gcc -o $@ -c $^ $(CFLAGS)

clean:
	@rm -rf *~ bin lib obj
