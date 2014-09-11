/* 
 * This file includes functions to create a ramp pattern from variable inputs.
 * For example, say I want to take a 
 * 
*/


#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include "check-timing.h"
#include "ramp.h"


void RP_print(RampParameter* rp) {
    printf("yi          %.2f\n", rp->yi);
    printf("yf          %.2f\n", rp->yf);
    printf("dydt        %.2f\n", rp->dydt);
    printf("dy          %.2f\n", rp->dy);
    printf("y_Delta_min %.1e\n", rp->y_Delta_min);
}

void* RP_init(double yi, double yf, double dydt, double dy) {
    RampParameter* rp = malloc(sizeof(RampParameter));
    rp->yi = yi;
    rp->yf = yf;
    rp->dydt = dydt;
    rp->dy = dy;
    rp->y_Delta_min = 0.001;
    return rp;
}

void RP_destroy(RampParameter* rp) {
    free(rp);
}

int RP_check(RampParameter* rp, TimingParameter* tp) {
    // Define the difference between y_initial and y_final
    double y_Delta = fmax(fabs(rp->yf - rp->yi), rp->y_Delta_min);
    if (rp->dydt > 0) {
        tp->T = y_Delta * rp->dydt;
    }
    if (rp->dy > 0) {
        tp->N = y_Delta / rp->dy;
    }
    // Now everything is set up; just need to send this to the timing parameter
    int status = TP_check(tp);
    return status;
}