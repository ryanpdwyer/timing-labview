/// A main function to run RP_check
#include <stdlib.h>
#include <stdio.h>
#include "check-timing.h"
#include "ramp.h"

int main(int argc, char const *argv[])
{
    if (argc < 5) {
        printf("Please input yi yf dydt dy.\n");
    }
    else {
        double yi, yf, dydt, dy;
        yi = atof(argv[1]);
        yf = atof(argv[2]);
        dydt = atof(argv[3]);
        dy = atof(argv[4]);
        TimingParameter* tp = TP_init(0, 0, 0,0);
        RampParameter* rp = RP_init(yi, yf, dydt, dy);
        int status = RP_check(rp, tp);
        RP_print(rp);
        TP_print(tp);
        printf("status: %d\n", status);
        free(tp);
        free(rp);
    }
    return 0;
}
