// AUTOGENERATED test_x_2_mu.h -> to '#include' in test_x_2.c

// test functions

void test_summ_int(void);
void test_summ_float(void);
void test_summ_mix(void);

// test runner

void
mu_run_tests(void)
{
    mu_verify(test_summ_int);
    mu_verify(test_summ_float);
    mu_verify(test_summ_mix);
}
