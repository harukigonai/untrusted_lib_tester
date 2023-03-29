CC = gcc
CFLAGS = -g -Wall -fPIC
LDFLAGS = -g -shared
LDLIBS = -ldl

.PHONY: all
all: clean libfunc_wrapper.so

.PHONY: clean
clean:
	rm -f *.o *.so *~

%.so:
	$(LINK.c) $(LDFLAGS) $^ -o $@ $(LDLIBS)

libfunc_wrapper.so: libfunc.o

libfunc.o: libfunc.c

