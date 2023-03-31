CC = gcc
CFLAGS = -g -Wall -fPIC
LDFLAGS = -g -shared
LDLIBS = -ldl

.PHONY: all
all: clean libmalloc.so

.PHONY: clean
clean:
	rm -f *.o *.so *~

%.so:
	$(LINK.c) $(LDFLAGS) $^ -o $@ $(LDLIBS)

libmalloc.so: malloc.o

malloc.o: malloc.c
