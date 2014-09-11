CC=gcc-4.8
CFLAGS=-Wall -lm

check-timing.o: check-timing.h

ramp.o: check-timing.h ramp.h

tests.o: check-timing.h ramp.h

tests: check-timing.o ramp.o tests.o

.PHONY:
	clean

clean:
	rm check-timing.o
	rm ramp.o
	rm tests.o
	rm tests