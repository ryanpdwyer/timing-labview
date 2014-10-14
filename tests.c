#include <stdlib.h>
#include "minunit.h"
#include "check-timing.h"
#include "ramp.h"

// A helper function to debug a TimingParameter using minunit.
void check_tp_state(TimingParameter* tp, double expected[]) {
    mu_assert_double_eq(expected[0], tp->fs);
    mu_assert_double_eq(expected[1], tp->dt);
    mu_assert_double_eq(expected[2], tp->N);
    mu_assert_double_eq(expected[3], tp->T);
}

void check_rp_state(RampParameter* rp, double expected[]) {
    mu_assert_double_eq(expected[0], rp->yi);
    mu_assert_double_eq(expected[1], rp->yf);
    mu_assert_double_eq(expected[2], rp->dydt);
    mu_assert_double_eq(expected[3], rp->dy);
}

// Test that creating a timing parameter on the heap works as advertised.
MU_TEST(test_TP_init) {
    TimingParameter* tp = TP_init(25.0, 0.04, 100, 4);
    double expected[] = {25, 0.04, 100, 4};
    check_tp_state(tp, expected);
    free(tp);
}

// Test fs_dt_consistent with a number of test cases.
MU_TEST(test_fs_dt_consistent) {
    int status;
    
    // Both defined
    TimingParameter* tp_both = TP_init(2, 0.5, 0, 0);
    double expected_both[] = {2, 0.5, 0, 0};
    status = fs_dt_consistent(tp_both);
    check_tp_state(tp_both, expected_both);
    free(tp_both);

    // fs N defined
    TimingParameter* tp_fs = TP_init(100, 0, 10, 0);
    // T still 0 afterwards because only checking fs_dt_consistent
    double expected_fs[] = {100, 0.01, 10, 0};
    status = fs_dt_consistent(tp_fs);
    check_tp_state(tp_fs, expected_fs);
    free(tp_fs);

    // dt N defined
    TimingParameter* tp_dt_N = TP_init(0, 0.1, 10, 0);
    double expected_dt[] = {10, 0.1, 10, 0};
    status = fs_dt_consistent(tp_dt_N);
    check_tp_state(tp_dt_N, expected_dt);
    free(tp_dt_N);
    
    // fs N defined
    TimingParameter* tp_fs_N = TP_init(250, 0, 500, 0);
    double expected_fs_N[] = {250, 0.004, 500, 0};
    status = fs_dt_consistent(tp_fs_N);
    check_tp_state(tp_fs_N, expected_fs_N);
    free(tp_fs_N);

    // Only N defined
    TimingParameter* tp_neither = TP_init(0, 0, 10, 0);
    double expected_neither[] = {0, 0, 10, 0};
    status = fs_dt_consistent(tp_neither);
    check_tp_state(tp_neither, expected_neither);
    free(tp_neither);
    mu_assert_int_eq(2, status);

}

// Test TP_check with the same test cases.
MU_TEST(test_TP_check){
    int status;
    // Both defined
    TimingParameter* tp_both = TP_init(2, 0.5, 0, 0);
    double expected_both[] = {2, 0.5, 0, 0};
    status = TP_check(tp_both);
    check_tp_state(tp_both, expected_both);
    free(tp_both);
    mu_assert_int_eq(-1, status);

    // fs N defined
    TimingParameter* tp_fs = TP_init(100, 0, 10, 0);
    // T still 0 afterwards because only checking fs_dt_consistent
    double expected_fs[] = {100, 0.01, 10, 0.1};
    status = TP_check(tp_fs);
    check_tp_state(tp_fs, expected_fs);
    free(tp_fs);

    // dt N defined
    TimingParameter* tp_dt_N = TP_init(0, 0.1, 10, 0);
    double expected_dt[] = {10, 0.1, 10, 1};
    status = TP_check(tp_dt_N);
    check_tp_state(tp_dt_N, expected_dt);
    free(tp_dt_N);

    // fs N defined
    TimingParameter* tp_fs_N = TP_init(250, 0, 500, 0);
    double expected_fs_N[] = {250, 0.004, 500, 2};
    status = TP_check(tp_fs_N);
    check_tp_state(tp_fs_N, expected_fs_N);
    free(tp_fs_N);

    // Only N defined
    TimingParameter* tp_neither = TP_init(0, 0, 10, 0);
    double expected_neither[] = {0, 0, 10, 0};
    status = TP_check(tp_neither);
    check_tp_state(tp_neither, expected_neither);
    free(tp_neither);
    mu_assert_int_eq(-1, status);
}


MU_TEST(test_RP_check) {
    int status;
    // Normal case
    TimingParameter* tp1 = TP_init(0, 0, 0, 0);
    RampParameter* rp1 = RP_init(0, 10, 2, 0.01);
    
    status = RP_check(rp1, tp1);
    mu_assert_int_eq(0, status);

    double tp_exp[] = {200, 0.005, 1000, 5};
    double rp_exp[] = {0, 10, 2, 0.01};
    
    check_tp_state(tp1, tp_exp);
    check_rp_state(rp1, rp_exp);

    free(tp1);
    free(rp1);

    // Case that requires N to be forced to 2.
    TimingParameter* tp2 = TP_init(0, 0, 0, 0);
    RampParameter* rp2 = RP_init(0, 0, 2, 0.01);
    
    status = RP_check(rp2, tp2);
    mu_assert_int_eq(0, status);

    double tp_exp2[] = {4000, 0.00025, 2, 0.0005};
    double rp_exp2[] = {0, 0, 2, 0.01};

    check_tp_state(tp2, tp_exp2);
    check_rp_state(rp2, rp_exp2);

    free(tp2);
    free(rp2);

}

// Set up the test suite.
MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_TP_init);
    MU_RUN_TEST(test_fs_dt_consistent);
    MU_RUN_TEST(test_TP_check);
    MU_RUN_TEST(test_RP_check);
}

// Run the test suite, and report the results.
int main(void)
{

    MU_RUN_SUITE(test_suite);
    MU_REPORT();

    return minunit_fail;
}
