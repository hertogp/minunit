// file: test_x.c -- a (minunit) test file for 'x.c'

#include "x.h"            // unit under test
#include "minunit.h"      // provides mu test macros
#include "test_x_2_mu.h"  // provides main and the test runner

void
test_summ_int(void)
{
    int a = 11, b = 32;
    int answer = summ(a, b);

    mu_eq(42, answer, "%d");
    mu_assert(42 == answer);
    mu_true(42 == answer);
}

void test_summ_float(void)
{
    double a = 1.0, b = 3.2;
    double answer = summ(a, b);
    mu_equal(4.2, answer, "expected 4.2, got %f instead", answer);
    mu_eq(4.2, answer, "%2.1f");
}

void test_summ_mix(void)
{
    int a = 10;
    double b = 32;
    int answer = summ(a, b);
    mu_true(42 == answer);
    mu_eq(41, answer, "%d");
}

// Note: no main here (see test_x_mu.h)
// - just write the test_functions ...
