// file: test_x.c -- a (minunit) test file for 'x.c'

#include "x.h"         // unit under test
#include "minunit.h"   // provides mu test macros
#include "test_x_mu.h" // provides main and the test runner

void test_xyz(void)
{
    int a=1, b=1;

    // these fail
    mu_assert(summ(a,b) == 3);
    mu_equal(summ(a,b), 3, "a+b really should equal %d!!", 3);
    mu_eq(3, summ(a,b), "%d");
    mu_true(summ(a,b) == 3);
    mu_false(summ(a,b) == 2);

    // these succeed
    mu_assert(a+b == 2);
    mu_equal(a+b, 2, "1+1 really should equal %d", 2);
    mu_eq(2, a+b, "%d");
    mu_true(a+b == 2);
    mu_false(a+b == 3);

}

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
