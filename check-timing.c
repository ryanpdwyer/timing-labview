// check-timing.c
// Ryan Dwyer
// 2014-08-30

// This file implements a structure, TimingParameter, defined in check-timing.h.
// It is designed to allow for easy conversion between sampling frequency (fs),
// time between data points (dt), number of samples (N),
// and total sampling time (T) in LabView.

// The structure also defines

// TODO:
// Define an enum to deal with statuses.
// Use minunit to do some unittests.


// We will compile this as a dll to use in Labview. See
// https://cygwin.com/cygwin-ug-net/dll.html
// Steps:
// gcc -c check-timing.c
// gcc -shared -o check-timing.dll check-timing.o


#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include "check-timing.h"
#include "minunit.h"

// Initialize a TimingParameter on the heap.
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

// Free a timing parameter created on the heap.
void TP_destroy(TimingParameter *tp) {
    free(tp);
}

MU_TEST(test_TP_init) {
    TimingParameter* tp = TP_init(25.0, 0.04, 100, 4);
    mu_assert_double_eq(25.0, tp->fs);
    mu_assert_double_eq(0.04, tp->dt);
    mu_assert_double_eq(100, tp->N);
    mu_assert_double_eq(4, tp->T);
    TP_destroy(tp);
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
            tp->N = tp->T * tp->fs;
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

void check_tp_state(TimingParameter* tp, double expected[]) {
    mu_assert_double_eq(tp->fs, expected[0]);
    mu_assert_double_eq(tp->dt, expected[1]);
    mu_assert_double_eq(tp->N, expected[2]);
    mu_assert_double_eq(tp->T, expected[3]);
}

int TP_check(TimingParameter* tp) {
    int status = fs_dt_consistent(tp);
    int status2 = N_T_consistent(tp);
    return status2;
}

MU_TEST(test_fs_dt_consistent) {
    int status;
    // Both defined
    TimingParameter* tp_both = TP_init(2, 0.5, 0, 0);
    double expected_both[] = {2, 0.5, 0, 0};
    status = fs_dt_consistent(tp_both);
    check_tp_state(tp_both, expected_both);
    TP_destroy(tp_both);


    // fs N defined
    TimingParameter* tp_fs = TP_init(100, 0, 10, 0);
    // T still 0 afterwards because only checking fs_dt_consistent
    double expected_fs[] = {100, 0.01, 10, 0};
    status = fs_dt_consistent(tp_fs);
    check_tp_state(tp_fs, expected_fs);
    TP_destroy(tp_fs);

    TimingParameter* tp_dt = TP_init(0, 0.1, 10, 0);
    double expected_dt[] = {10, 0.1, 10, 0};
    status = fs_dt_consistent(tp_dt);
    check_tp_state(tp_dt, expected_dt);
    TP_destroy(tp_dt);

    TimingParameter* tp_fs_N = TP_init(250, 0, 500, 0);
    double expected_fs_N[] = {250, 0.004, 500, 0};
    status = fs_dt_consistent(tp_fs_N);
    check_tp_state(tp_fs_N, expected_fs_N);
    TP_destroy(tp_fs_N);

    TimingParameter* tp_neither = TP_init(0, 0, 10, 0);
    double expected_neither[] = {0, 0, 10, 0};
    status = fs_dt_consistent(tp_neither);
    check_tp_state(tp_neither, expected_neither);
    TP_destroy(tp_neither);
    mu_assert(status == 2, "Should have status 2 (neither tp nor fs defined");

}

MU_TEST(test_check_tp_case){
    int status;
    // Both defined
    TimingParameter* tp_both = TP_init(2, 0.5, 0, 0);
    double expected_both[] = {2, 0.5, 0, 0};
    status = TP_check(tp_both);
    check_tp_state(tp_both, expected_both);
    TP_destroy(tp_both);
    mu_assert_int_eq(-1, status);


    // fs N defined
    TimingParameter* tp_fs = TP_init(100, 0, 10, 0);
    // T still 0 afterwards because only checking fs_dt_consistent
    double expected_fs[] = {100, 0.01, 10, 0.1};
    status = TP_check(tp_fs);
    check_tp_state(tp_fs, expected_fs);
    TP_destroy(tp_fs);

    TimingParameter* tp_dt = TP_init(0, 0.1, 10, 0);
    double expected_dt[] = {10, 0.1, 10, 1};
    status = TP_check(tp_dt);
    check_tp_state(tp_dt, expected_dt);
    TP_destroy(tp_dt);

    TimingParameter* tp_fs_N = TP_init(250, 0, 500, 0);
    double expected_fs_N[] = {250, 0.004, 500, 2};
    status = TP_check(tp_fs_N);
    check_tp_state(tp_fs_N, expected_fs_N);
    TP_destroy(tp_fs_N);

    TimingParameter* tp_neither = TP_init(0, 0, 10, 0);
    double expected_neither[] = {0, 0, 10, 0};
    status = TP_check(tp_neither);
    check_tp_state(tp_neither, expected_neither);
    TP_destroy(tp_neither);
    mu_assert_int_eq(-1, status);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_TP_init);
    MU_RUN_TEST(test_fs_dt_consistent);
    MU_RUN_TEST(test_check_tp_case);
}

int main(int argc, char const *argv[])
{

    MU_RUN_SUITE(test_suite);
    MU_REPORT();

    return 0;
}