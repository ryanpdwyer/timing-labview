CFLAGS=-Wall -Wextra -O3 -pedantic -std=gnu99
LDLIBS=-lm
OBJS=check-timing.o ramp.o tests.o evaltp.o evalramp.o
EXECUTABLES=tests evaltp evalrampmak
SHARED=-shared -static-libgcc

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
LDLIBS += -Wl,--no-as-needed -lrt
SHARED = -shared
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
	$(CC) $(CFLAGS)  -c check-timing.c ramp.c tests.c $(LDLIBS)
	$(CC) -o check-timing.dll check-timing.o ramp.o tests.o $(LDLIBS)
	$(CC) -o ramp.dll check-timing.o ramp.o tests.o $(LDLIBS)

evaltp.o: check-timing.h

check-timing.o: check-timing.h

ramp.o: check-timing.h ramp.h

evalramp.o: check-timing.h ramp.h

tests.o: check-timing.h ramp.h minunit.h
	$(CC) $(CFLAGS) -c -o tests.o tests.c $(LDLIBS)

.PHONY:
	clean

clean:
	rm $(OBJS) $(EXECUTABLES)
