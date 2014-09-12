/// A main function to run TP_check
#include <stdlib.h>
#include <stdio.h>
#include "check-timing.h"

int main(int argc, char const *argv[])
{
    if (argc < 5) {
        printf("Please input fs dt N T.\n");
    }
    else {
        double fs, dt, N, T;
        fs = atof(argv[1]);
        dt = atof(argv[2]);
        N = atof(argv[3]);
        T = atof(argv[4]);
        TimingParameter* tp = TP_init(fs, dt, N, T);
        int status = TP_check(tp);
        TP_print(tp);
        printf("status: %d\n", status);
        free(tp);
    }
    return 0;
}