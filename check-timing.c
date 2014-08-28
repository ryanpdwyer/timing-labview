#include <stdio.h>

typedef struct TimingParameters {
    double fs;
    double dt;
    int N;
    double T;
} TimingParameter;


const TimingParameter TIMING_PARAMETER_INIT = {
    0, 0, 0, 0
};

int fs_dt_consistent(TimingParameter tp) {
    int status = 0;
    if (tp.fs > 0 && tp.dt > 0) {
        printf("both greater than 0\n");
    }
    else if (tp.fs > 0 && tp.dt <= 0) {
        printf("Will use fs\n");
        tp.dt = 1.0 / tp.fs;
    }
    else if (tp.fs <= 0 && tp.dt > 0) {
        printf("Will use dt\n");
        tp.fs = 1.0 / tp.dt;
    }
    else if (tp.fs <= 0 && tp.dt <= 0) {
        printf("What to do?\n");
        status = -1;
    }
    return status;
}

void print_TP(TimingParameter tp) {
    printf("fs %f\n", tp.fs);
    printf("dt %f\n", tp.dt);
    printf("N %f\n", tp.N);
    printf("T %f\n", tp.T);
}

int verify_TimingParameters() {
    return 0;
}

int main(int argc, char const *argv[])
{
    int status;
    printf("Try both defined\n");
    TimingParameter tp_both = TIMING_PARAMETER_INIT;
    tp_both.fs = 2;
    tp_both.dt = 1;
    status = fs_dt_consistent(tp_both);
    print_TP(tp_both);
    printf("%d\n", status);
    printf("Try fs case\n");
    TimingParameter tp_fs = TIMING_PARAMETER_INIT;
    tp_fs.fs = 2;
    status = fs_dt_consistent(tp_fs);
    printf("%d\n", status);
    print_TP(tp_fs);
    printf("Try dt case\n");
    TimingParameter tp_dt = TIMING_PARAMETER_INIT;
    tp_dt.dt = 1;
    status = fs_dt_consistent(tp_dt);
    print_TP(tp_dt);
    printf("%d\n", status);
    printf("Try neither case\n");
    TimingParameter tp_neither = TIMING_PARAMETER_INIT;
    status = fs_dt_consistent(tp_neither);
    print_TP(tp_neither);
    printf("%d\n", status);
    
    return 0;
}