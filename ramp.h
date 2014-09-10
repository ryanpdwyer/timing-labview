typedef struct RampParameters {
    double fs;
    double dt;
    double N;
    double T;
    double eps;
} RampParameter;

double* ramp(RampParameter* rp);