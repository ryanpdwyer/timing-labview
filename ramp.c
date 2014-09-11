/* 
 * This file includes functions to create a ramp pattern from variable inputs.
 * For example, say I want to take a 
 * 
*/


#include <stdio.h>
#include "check-timing.h"
#include "ramp.h"


void RP_print(RampParameter rp) {
    printf("yi          %.2f\n", rp.yi);
    printf("yf          %.2f\n", rp.yf);
    printf("dydt        %.2f\n", rp.dydt);
    printf("dy          %.2f\n", rp.dy);
    printf("y_Delta_min %.1e\n", rp.y_Delta_min);
}

int RP_check(RampParameter* rp, TimingParameter* tp) {
    int status = 0;
    status = TP_check(tp);
    return status;
}