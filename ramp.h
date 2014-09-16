#pragma once

typedef struct RampParameters {
    double yi;
    double yf;
    double dydt;
    double dy;
    double y_Delta_min; // The minimum difference between yf and yi, for
                        // calculating number of points, rate, etc.
} RampParameter;

void RP_print(RampParameter* rp);

void* RP_init(double yi, double yf, double dydt, double dy);

int RP_check(RampParameter* rp, TimingParameter* tp);
