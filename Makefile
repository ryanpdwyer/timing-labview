CC=gcc-4.8
CFLAGS=-Wall -lm

check-timing.o:
	$(CC) $(CFLAGS) -c check-timing.c

ramp.o:
	$(CC) $(CFLAGS) -c ramp.c

tests.o:
	$(CC) $(CFLAGS) -c tests.c

tests: check-timing.o ramp.o tests.o
	$(CC) $(CFLAGS) -o tests tests.o check-timing.o ramp.o

.PHONY: 
	clean

clean:
	rm check-timing
	rm check-timing.o
	rm ramp
	rm ramp.o
	rm tests