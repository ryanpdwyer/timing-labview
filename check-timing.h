// Check Timing Header
// Include definitions for the function and strucuture used in the LabView
// code.
#ifndef __CHECKTIMING_H__
#define __CHECKTIMING_H__

typedef struct TimingParameters {
    double fs;
    double dt;
    double N;
    double T;
    double eps;
} TimingParameter;

void* TP_init(double fs, double dt, double N, double T);

int fs_dt_consistent(TimingParameter *tp);

int N_T_consistent(TimingParameter *tp);

int TP_check(TimingParameter* tp);

int check_tp_case(TimingParameter* tp, char message[]);

void check_tp_state(TimingParameter* tp, double expected[]);

void TP_print(TimingParameter* tp);

#endif /* __CHECKTIMING_H__ */