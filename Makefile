CC=gcc
CFLAGS=-Wall -Wextra -O3 -pedantic
LDLIBS=-lm
OBJS=check-timing.o ramp.o tests.o tp.o
EXECUTABLES=tests tp



all: tests tp

tests: check-timing.o ramp.o tests.o

tp: check-timing.o tp.o

# Note: This target will only compile on Windows using msys.
labview: check-timing.o ramp.o tests.o
	$(CC) $(CFLAGS) $(LDLIBS) -c check-timing.c ramp.c tests.c
	$(CC) $(LDLIBS) -shared -static-libgcc -o check-timing.dll check-timing.o ramp.o tests.o
	(CC) $(LDLIBS) -shared -static-libgcc -o ramp.dll check-timing.o ramp.o tests.o

tp.o: check-timing.h

check-timing.o: check-timing.h

ramp.o: check-timing.h ramp.h

tests.o: check-timing.h ramp.h

.PHONY:
	clean

clean:
	rm $(OBJS) $(EXECUTABLES)
