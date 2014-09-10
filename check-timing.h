// Check Timing Header
// Include definitions for the function and strucuture used in the LabView
// code.
typedef struct TimingParameters {
    double fs;
    double dt;
    double N;
    double T;
    double eps;
} TimingParameter;

int TP_check(TimingParameter* tp);