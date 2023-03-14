CC = gcc
CFLAGS = -g -Wall
LDLIBS = -lm

.PHONY: all
all: clean init_2 main libfunc

.PHONY: clean
clean:
	rm -f *.o *~ init_2 main && cd lib_wrapper && $(MAKE) clean

init_2: init_2.o
	$(CC) -static $(CFLAGS) $^ -o $@ 

init_2.o: init_2.c

main: main.o

main.o: main.c

libfunc:
	cd lib_wrapper && $(MAKE) all
