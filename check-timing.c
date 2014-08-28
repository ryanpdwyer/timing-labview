#include <stdio.h>
#include <tgmath.h>

typedef struct TimingParameters {
    double fs;
    double dt;
    int N;
    double T;
    double eps;
} TimingParameter;


const TimingParameter TIMING_PARAMETER_INIT = {
    0, 0, 0, 0, 0.1
};

int fs_dt_consistent(TimingParameter *tp) {
    int status = 0;
    if (tp->fs > 0 && tp->dt > 0) {
        printf("both\n");
        float fs_dt = 1.0 / tp->dt;
        if (fabs(fs_dt - tp->dt) <= tp->eps) {
            tp->dt = 1 / tp->fs;
            printf("%d\n", tp->N);
        }
        else {
            status = 1; // Timing parameters don't match
            tp->dt = 1 / tp->fs;
        }
    }
    else if (tp->fs > 0 && tp->dt <= 0) {
        printf("fs\n");
        tp->dt = 1.0 / tp->fs;
    }
    else if (tp->fs <= 0 && tp->dt > 0) {
        printf("dt\n");
        tp->fs = 1.0 / tp->dt;
    }
    else if (tp->fs <= 0 && tp->dt <= 0) {
        printf("Neither\n");
        status = -1;
    }
    return status;
}

void print_TP(TimingParameter *tp) {
    printf("fs %f\n", tp->fs);
    printf("dt %f\n", tp->dt);
    printf("N %f\n", tp->N);
    printf("T %f\n", tp->T);
    printf("eps %f\n", tp->eps);
}

int verify_TimingParameters() {
    return 0;
}

int main(int argc, char const *argv[])
{
    int status;
    printf("Try both defined\n");
    TimingParameter tp_both;
    print_TP(&tp_both);
    tp_both.fs = 2;
    tp_both.dt = 0.5;
    tp_both.N = 10;
    printf("Before\n");
    print_TP(&tp_both);
    status = fs_dt_consistent(&tp_both);
    print_TP(&tp_both);
    printf("%d\n", status);
    printf("Try fs case\n");
    TimingParameter tp_fs = TIMING_PARAMETER_INIT;
    tp_fs.fs = 10;
    tp_fs.N = 10;
    status = fs_dt_consistent(&tp_fs);
    printf("%d\n", status);
    print_TP(&tp_fs);
    printf("Try dt case\n");
    TimingParameter tp_dt = TIMING_PARAMETER_INIT;
    tp_dt.dt = 0.1;
    tp_dt.N = 10;
    print_TP(&tp_dt);
    status = fs_dt_consistent(&tp_dt);
    print_TP(&tp_dt);
    printf("%d\n", status);
    printf("Try neither case\n");
    TimingParameter tp_neither = TIMING_PARAMETER_INIT;
    tp_neither.N = 10;
    status = fs_dt_consistent(&tp_neither);
    print_TP(&tp_neither);
    printf("%d\n", status);
    
    return 0;
}