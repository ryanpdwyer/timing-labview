/* 
 * This file includes functions to create a ramp pattern from variable inputs.
 * For example, say I want to take a 
 * 
*/


#include <stdio.h>
#include "ramp.h"
#include "minunit.h"

double* ramp(RampParameter* rp) {
    double ramp_pattern = {0, 0.2};
    return &ramp_pattern;
}

void main(int argc, char const *argv[])
{
    printf("Main function\n");
}