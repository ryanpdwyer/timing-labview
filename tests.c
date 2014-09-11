#include "minunit.h"
#include "check-timing.h"
#include "ramp.h"

// A helper function to debug a TimingParameter using minunit.
void check_tp_state(TimingParameter* tp, double expected[]) {
    mu_assert_double_eq(tp->fs, expected[0]);
    mu_assert_double_eq(tp->dt, expected[1]);
    mu_assert_double_eq(tp->N, expected[2]);
    mu_assert_double_eq(tp->T, expected[3]);
}

// Test that creating a timing parameter on the heap works as advertised.
MU_TEST(test_TP_init) {
    TimingParameter* tp = TP_init(25.0, 0.04, 100, 4);
    mu_assert_double_eq(25.0, tp->fs);
    mu_assert_double_eq(0.04, tp->dt);
    mu_assert_double_eq(100, tp->N);
    mu_assert_double_eq(4, tp->T);
    TP_destroy(tp);
}

// Test fs_dt_consistent with a number of test cases.
MU_TEST(test_fs_dt_consistent) {
    int status;
    
    // Both defined
    TimingParameter* tp_both = TP_init(2, 0.5, 0, 0);
    double expected_both[] = {2, 0.5, 0, 0};
    status = fs_dt_consistent(tp_both);
    check_tp_state(tp_both, expected_both);
    TP_destroy(tp_both);

    // fs N defined
    TimingParameter* tp_fs = TP_init(100, 0, 10, 0);
    // T still 0 afterwards because only checking fs_dt_consistent
    double expected_fs[] = {100, 0.01, 10, 0};
    status = fs_dt_consistent(tp_fs);
    check_tp_state(tp_fs, expected_fs);
    TP_destroy(tp_fs);

    // dt N defined
    TimingParameter* tp_dt_N = TP_init(0, 0.1, 10, 0);
    double expected_dt[] = {10, 0.1, 10, 0};
    status = fs_dt_consistent(tp_dt_N);
    check_tp_state(tp_dt_N, expected_dt);
    TP_destroy(tp_dt_N);
    
    // fs N defined
    TimingParameter* tp_fs_N = TP_init(250, 0, 500, 0);
    double expected_fs_N[] = {250, 0.004, 500, 0};
    status = fs_dt_consistent(tp_fs_N);
    check_tp_state(tp_fs_N, expected_fs_N);
    TP_destroy(tp_fs_N);

    // Only N defined
    TimingParameter* tp_neither = TP_init(0, 0, 10, 0);
    double expected_neither[] = {0, 0, 10, 0};
    status = fs_dt_consistent(tp_neither);
    check_tp_state(tp_neither, expected_neither);
    TP_destroy(tp_neither);
    mu_assert(2, status);

}

// Test TP_check with the same test cases.
MU_TEST(test_TP_check){
    int status;
    // Both defined
    TimingParameter* tp_both = TP_init(2, 0.5, 0, 0);
    double expected_both[] = {2, 0.5, 0, 0};
    status = TP_check(tp_both);
    check_tp_state(tp_both, expected_both);
    TP_destroy(tp_both);
    mu_assert_int_eq(-1, status);

    // fs N defined
    TimingParameter* tp_fs = TP_init(100, 0, 10, 0);
    // T still 0 afterwards because only checking fs_dt_consistent
    double expected_fs[] = {100, 0.01, 10, 0.1};
    status = TP_check(tp_fs);
    check_tp_state(tp_fs, expected_fs);
    TP_destroy(tp_fs);

    // dt N defined
    TimingParameter* tp_dt_N = TP_init(0, 0.1, 10, 0);
    double expected_dt[] = {10, 0.1, 10, 1};
    status = TP_check(tp_dt_N);
    check_tp_state(tp_dt_N, expected_dt);
    TP_destroy(tp_dt_N);

    // fs N defined
    TimingParameter* tp_fs_N = TP_init(250, 0, 500, 0);
    double expected_fs_N[] = {250, 0.004, 500, 2};
    status = TP_check(tp_fs_N);
    check_tp_state(tp_fs_N, expected_fs_N);
    TP_destroy(tp_fs_N);

    // Only N defined
    TimingParameter* tp_neither = TP_init(0, 0, 10, 0);
    double expected_neither[] = {0, 0, 10, 0};
    status = TP_check(tp_neither);
    check_tp_state(tp_neither, expected_neither);
    TP_destroy(tp_neither);
    mu_assert_int_eq(-1, status);
}


// Set up the test suite.
MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_TP_init);
    MU_RUN_TEST(test_fs_dt_consistent);
    MU_RUN_TEST(test_TP_check);
}

// Run the test suite, and report the results.
int main(int argc, char const *argv[])
{


    MU_RUN_SUITE(test_suite);
    MU_REPORT();

    return 0;
}