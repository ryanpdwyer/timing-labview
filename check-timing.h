typedef struct TimingParameters {
    double fs;
    double dt;
    double N;
    double T;
    double eps;
} TimingParameter;

int TP_check(TimingParameter* tp);