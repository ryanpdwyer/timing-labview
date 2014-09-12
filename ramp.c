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

/// Print the contents of the RampParameter.
void RP_print(RampParameter* rp) {
    printf("yi          %.2f\n", rp->yi);
    printf("yf          %.2f\n", rp->yf);
    printf("dydt        %.2f\n", rp->dydt);
    printf("dy          %.2f\n", rp->dy);
    printf("y_Delta_min %.1e\n", rp->y_Delta_min);
}

// Initialize a RampParameter on the heap.
void* RP_init(double yi, double yf, double dydt, double dy) {
    RampParameter* rp = malloc(sizeof(RampParameter));
    rp->yi = yi;
    rp->yf = yf;
    rp->dydt = dydt;
    rp->dy = dy;
    rp->y_Delta_min = 0.001;
    return rp;
}

int RP_check(RampParameter* rp, TimingParameter* tp) {
    // Define the difference between y_initial and y_final
    double y_Delta = fmax(fabs(rp->yf - rp->yi), rp->y_Delta_min);
    if (rp->dydt > 0) {
        tp->T = y_Delta / rp->dydt;
    }
    if (rp->dy > 0) {
        double N = floor(y_Delta / rp->dy + 0.5);
        // N must be greater than or equal to 2 for a ramp pattern to make sense.
        tp->N = fmax(N, 2);
    }
    // Now everything is set up; just need to send this to the timing parameter
    int status = TP_check(tp);
    return status;
}