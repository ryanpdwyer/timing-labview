#include <stdlib.h>
#include <stdio.h>
#include "check-timing.h"

int main(int argc, char const *argv[])
{
    if (argc < 4) {
        printf("Please input fs dt N T.\n");
    }
    else {
        double fs, dt, N, T;
        fs = atof(argv[1]);
        dt = atof(argv[2]);
        N = atof(argv[3]);
        T = atof(argv[4]);
        TimingParameter* tp = TP_init(fs, dt, N, T);
        TP_check(tp);
        TP_print(tp);
        TP_destroy(tp);
    }
    return 0;
}