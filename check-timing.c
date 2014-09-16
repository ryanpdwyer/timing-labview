/// check-timing.c
/// Ryan Dwyer
/// 2014-08-30

/// This file implements a structure, TimingParameter, defined in
/// check-timing.h.
/// It is designed to allow for easy conversion between sampling frequency (fs),
/// time between data points (dt), number of samples (N), and total sampling
/// time (T) in LabView.
/// 
/// It uses minunit (http://github.com/siu/minunit) for unittests.
/// To run the tests,
/// 1. Build using make
///     make tests
/// 
/// 2. Run tests
///      ./tests
///
/// We will compile this as a dll to use in Labview. See
/// https://cygwin.com/cygwin-ug-net/dll.html
/// The default instructions there no longer worked for me after switching to
/// using minunit for unittests.
/// Steps:
///      gcc -lm -static-libgcc -c check-timing.c ramp.c tests.c
///      gcc -shared -lm -static-libgcc -o timing-labview.dll check-timing.o ramp.o tests.o

/// TODO:
/// Define a function to get an error message from the status.

#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include "check-timing.h"

/// Initialize a TimingParameter on the heap.
void* TP_init(double fs, double dt, double N, double T) {
    TimingParameter* tp = malloc(sizeof(TimingParameter));
    tp->fs = fs;
    tp->dt = dt;
    tp->N = N;
    tp->T = T;
    tp->eps = 0.1; // Default to 0.1 for eps; used to determine if a
                   // TimingParameter is internally consistant.
    return tp;
}

// A helper function to print a TimingParameter
void TP_print(TimingParameter *tp) {
    printf("fs  %.3g\n", tp->fs);
    printf("dt  %.3g\n", tp->dt);
    printf("N   %.1f\n", tp->N);
    printf("T   %.2g\n", tp->T);
    printf("eps %.2g\n", tp->eps);
}

// One of two subfunctions that enforce consistancy on fs, dt, N and T.
int fs_dt_consistent(TimingParameter *tp) {
    int status = 0;
    int fs_defined = tp->fs > 0;
    int dt_defined = tp->dt > 0;
    if (fs_defined && dt_defined) {
        // Both defined; check that they are with the tolerance eps
        double fs_inferred = 1.0 / tp->dt;
        if (fabs(fs_inferred - tp->fs) < tp->eps) {
            // Make sure the timing parameters are actually consistent
            // Since fs is what is sent to the DAQ, we'll define dt in terms
            // of fs.
            tp->dt = 1.0 / tp->fs; 
        }
        else {
             // Timing parameters don't match. Leave the situation to be
            // cleaned up by the next function, or define dt in terms of fs?.
            status = 1;
            tp->dt = 1.0 / tp->fs;
        }
    }
    else if (fs_defined && !dt_defined) {
        // fs defined
        tp->dt = 1.0 / tp->fs;
    }
    else if (!fs_defined && dt_defined) {
        // dt defined
        tp->fs = 1.0 / tp->dt;
    }
    else if (!fs_defined && !dt_defined) {
        // Neither defined. Exit with status 2, and let N_T consistent deal
        // with this.
        status = 2;
    }
    return status;
}

// The other function which fills in missing parameters, and makes the entire 
// structure consistent.
int N_T_consistent(TimingParameter *tp) {
    // This function is designed to be called after fs_dt_consistent, and will
    // assign the rest of the unassigned parameters.
    int status = 0;
    int N_defined = tp->N > 0;
    int T_defined = tp->T > 0;
    int fs_defined = tp->fs > 0;
    int dt_defined = tp->dt > 0;

    if (N_defined && T_defined) {
        if (!fs_defined && !dt_defined) {
            // This is a normal case; define fs in terms of N and T, then
            // dt in terms of fs.
            tp->fs = tp->N / tp->T;
            tp->dt = 1.0 / tp->fs;
        }
        else if (fs_defined && dt_defined) {
            // This is complicated, but things are all defined, so just raise
            // a status and get out. Should probably have a function to check 
            // the cluster's consistency here.
            status = 3;
        }
        else {
            // The structure should not get into this state. Just raise a
            // status and get out.
            status = -2;
        }
    } else if (N_defined && !T_defined) {
        // Assume fs and dt are already defined. We default to always using fs
        if (fs_defined && dt_defined) {
            tp->T = tp->N / tp->fs;
        }
        else {
            // Again, this is not a well-defined state for TP.
            status = -1;
        }
    } else if (!N_defined && T_defined) {
        if (fs_defined && dt_defined) {
            tp->N = floor(tp->T * tp->fs + 0.5);
        }
        else {
            // Again, this is not a well-defined state for TP.
            status = -1;
        }
    } else if (!N_defined && !T_defined){
        // Not enough parameters defined; raise a real error
        status = -1;
    }
    return status;
}

// The main function that we will call in LabView.
// After calling this function, the TimingParameter should be in
// a self-consistent state, or have raised an error by returning a status < 0.
int TP_check(TimingParameter* tp) {
    int status;
    status = fs_dt_consistent(tp);
    // Ignoring the first status; only used for debugging currently.
    status = N_T_consistent(tp);
    return status;
}

// A helper function to debug TimingParameter by printing.
// Replaced by minunit unittests, but could still be useful for adding
// functionality.
int check_tp_case(TimingParameter* tp, char message[]) {
    int status;
    printf("------------------\n");
    printf("%s\n\n", message);
    printf("Before:\n");
    TP_print(tp);
    status = fs_dt_consistent(tp);
    printf("After fs_dt_consistent:\n");
    TP_print(tp);
    printf("Status: %d\n\n", status);
    status = N_T_consistent(tp);
    printf("After N_T_consistent:\n");
    TP_print(tp);
    printf("Status: %d\n\n", status);
    return status;
}
