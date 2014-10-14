CFLAGS=-Wall -Wextra -O3 -pedantic -std=c99
LDLIBS=-lm
OBJS=check-timing.o ramp.o tests.o evaltp.o evalramp.o
EXECUTABLES=tests evaltp evalrampmak

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
LDLIBS += -lrt
endif


all: tests evaltp evalramp labview

tests: check-timing.o ramp.o tests.o
	$(CC) check-timing.o ramp.o tests.o -o tests $(LDLIBS)

test:
	./tests && exit $$?

evaltp: check-timing.o evaltp.o

evalramp: evalramp.o check-timing.o ramp.o

# Note: This target will only compile on Windows using msys.
labview: check-timing.o ramp.o tests.o
	$(CC) $(CFLAGS) $(LDLIBS) -c check-timing.c ramp.c tests.c
	$(CC) $(LDLIBS) -shared -static-libgcc -o check-timing.dll check-timing.o ramp.o tests.o
	$(CC) $(LDLIBS) -shared -static-libgcc -o ramp.dll check-timing.o ramp.o tests.o

evaltp.o: check-timing.h

check-timing.o: check-timing.h

ramp.o: check-timing.h ramp.h

evalramp.o: check-timing.h ramp.h

tests.o: check-timing.h ramp.h minunit.h

.PHONY:
	clean

clean:
	rm $(OBJS) $(EXECUTABLES)
