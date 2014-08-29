#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

typedef struct TimingParameters {
    double fs;
    double dt;
    double N;
    double T;
    double eps;
} TimingParameter;


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

int fs_dt_consistent(TimingParameter *tp) {
    int status = 0;
    if (tp->fs > 0 && tp->dt > 0) {
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
    else if (tp->fs > 0 && tp->dt <= 0) {
        // fs defined
        tp->dt = 1.0 / tp->fs;
    }
    else if (tp->fs <= 0 && tp->dt > 0) {
        // dt defined
        tp->fs = 1.0 / tp->dt;
    }
    else if (tp->fs <= 0 && tp->dt <= 0) {
        // Neither defined. Exit with error status -1
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
    printf("After:\n");
    TP_print(tp);
    printf("Status: %d\n\n", status);
    return status;
}

int main(int argc, char const *argv[])
{
    int status;

    printf("int:       %d\n", sizeof(int));
    printf("long:      %d\n", sizeof(long));
    printf("long long: %d\n", sizeof(long long));
    printf("double:    %d\n", sizeof(double));

    // Cases below
    TimingParameter* tp_both = TP_init(2, 0.5, 0, 0);
    check_tp_case(tp_both, "Both defined");
    TP_destroy(tp_both);

    TimingParameter* tp_fs = TP_init(10, 0, 10, 0);
    check_tp_case(tp_fs, "fs defined");
    TP_destroy(tp_fs);

    TimingParameter* tp_dt = TP_init(0, 0.1, 10, 0);
    check_tp_case(tp_dt, "fs defined");
    TP_destroy(tp_dt);

    TimingParameter* tp_neither = TP_init(0, 0, 10, 0);
    check_tp_case(tp_neither, "Neither defined");
    TP_destroy(tp_neither);
    return 0;
}