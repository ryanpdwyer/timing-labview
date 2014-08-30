#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include "check-timing.h"

// We will compile this as a dll to use in Labview. See
// https://cygwin.com/cygwin-ug-net/dll.html
// Steps:
// gcc -c check-timing.c
// gcc -shared -o check-timing.dll check-timing.o

// Initialize a TimingParameter on the heap.
void* TP_init(double fs, double dt, double N, double T) {
    TimingParameter* tp = malloc(sizeof(TimingParameter));
    tp->fs = fs;
    tp->dt = dt;
    tp->N = N;
    tp->T = T;
    tp->eps = 0.1; // Default to 0.1 for eps
    return tp;
}

void TP_destroy(TimingParameter *tp) {
    free(tp);
}

// One of two subfunctions that enforce consistancy on fs, dt, N and T.

int fs_dt_consistent(TimingParameter *tp) {
    int status = 0;
    int fs_defined = tp->fs > 0;
    int dt_defined = tp->dt > 0;
    if (fs_defined && dt_defined) {
        // Both defined; check that they are with the tolerance eps
        float fs_dt = 1.0 / tp->dt;
        if (fabs(fs_dt - tp->dt) <= tp->eps) {
            tp->dt = 1 / tp->fs;
        }
        else {
            status = 1; // Timing parameters don't match
            tp->dt = 1 / tp->fs;
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
        // Neither defined. Exit with status 2; could be an error, could not.
        status = 2;
    }
    return status;
}

// The other function which fills in missing parameters, and makes the entire 
// structure consistent.

int N_T_consistent(TimingParameter *tp) {
    int status = 0;
    int N_defined = tp->N > 0;
    int T_defined = tp->T > 0;
    int fs_defined = tp->fs > 0;
    int dt_defined = tp->dt > 0;

    if (N_defined && T_defined) {
        if (!fs_defined && !dt_defined) {
            // We can just define all the variables here then.
            tp->fs = tp->N / tp->T;
            tp->dt = 1.0 / tp->fs;
        }
        else if (fs_defined && dt_defined) {
            // This is complicated, but things are all defined, so just raise
            // a status and get out.
            status = 3;
        }
        else {
            // Things are weird. Raise a status.
            status = 4;
        }
    } else if (N_defined && !T_defined) {
        // Assume fs and dt are already defined. We default to always using fs
        if (fs_defined && dt_defined) {
            tp->T = tp->N / tp->fs;
        }
        else {
            // Again, this is not a well-defined state for TP.
            status = 4;
        }
    } else if (!N_defined && T_defined) {
        if (fs_defined && dt_defined) {
            tp->N = tp->T * tp->fs;
        }
        else {
            // Again, this is not a well-defined state for TP.
            status = 4;
        }
    } else if (!N_defined && !T_defined){
        // Not enough parameters defined; raise a real error
        status = -1;
    }
    return status;
}

void TP_print(TimingParameter *tp) {
    printf("fs %f\n", tp->fs);
    printf("dt %f\n", tp->dt);
    printf("N %f\n", tp->N);
    printf("T %f\n", tp->T);
    printf("eps %f\n", tp->eps);
}

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

int TP_check(TimingParameter* tp) {
    int status = fs_dt_consistent(tp);
    int status2 = N_T_consistent(tp);
    return (status || status2);
}

int main(int argc, char const *argv[])
{

    // printf("int:       %d\n", sizeof(int));
    // printf("long:      %d\n", sizeof(long));
    // printf("long long: %d\n", sizeof(long long));
    // printf("double:    %d\n", sizeof(double));

    // Cases below
    TimingParameter* tp_both = TP_init(2, 0.5, 0, 0);
    check_tp_case(tp_both, "Both defined");
    TP_destroy(tp_both);

    TimingParameter* tp_fs = TP_init(100, 0, 10, 0);
    check_tp_case(tp_fs, "fs N defined");
    TP_destroy(tp_fs);

    TimingParameter* tp_dt = TP_init(0, 0.1, 10, 0);
    check_tp_case(tp_dt, "dt N defined");
    TP_destroy(tp_dt);

    TimingParameter* tp_fs_N = TP_init(250, 0, 500, 0);
    check_tp_case(tp_fs_N, "fs N defined");
    TP_destroy(tp_fs_N);

    TimingParameter* tp_neither = TP_init(0, 0, 10, 0);
    check_tp_case(tp_neither, "Neither defined");
    TP_destroy(tp_neither);
    return 0;
}