CC=gcc-4.8
CFLAGS=-Wall -lm
OBJS=check-timing.o ramp.o tests.o tp.o
EXECUTABLES=tests tp



all: tests tp

tests: check-timing.o ramp.o tests.o

tp: check-timing.o tp.o

tp.o: check-timing.h

check-timing.o: check-timing.h

ramp.o: check-timing.h ramp.h

tests.o: check-timing.h ramp.h

.PHONY:
	clean

clean:
	rm $(OBJS) $(EXECUTABLES)