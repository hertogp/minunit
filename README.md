# minunit

A working example of a minimalistic C unit testing 'framework', adapted from:
- [*minunit*](http://www.jera.com/techinfo/jtns/jtn003.html) by John Brewer, and
- [*tdd in c*](http://eradman.com/posts/tdd-in-c.html) by Eric Radman.

# Project setup:

```bash
.
├── LICENSE
├── Makefile                # assumes valgrind is installed
├── README.md
└── src                     # src tree
    ├── minunit.h           # the 'framework'
    ├── mu_header.sh        # helper to create _mu.h headers for unit tests
    ├── tst                 # the unit test files
    │   ├── test_x_1.c
    │   └── test_x_2.c
    ├── x.c                 # code to be tested
    └── x.h

2 directories, 9 files
```

Running `make`, compiles the `src/*.c` files into `bld/libx.so.1.0.1`, with a
`-soname=libx.so.1`.  Running `make test` runs all unit tests, compiling the
`src/tst/test_<t>.c` files into `tst/test_<t>` executables when needed.

Individual unit tests targets are derived from the unit test c-files, so `make
test_x_1` would run just that unit test. Finally `make clean` clears both the
`bld` and `tst` subdirectories.

The tree after `make test` (both unit tests ran):

```bash
.
├── bld                     # build dir
│   ├── libx.so -> libx.so.1.0.1
│   ├── libx.so.1 -> libx.so.1.0.1
│   ├── libx.so.1.0.1
│   ├── test_x_1_mu.h
│   ├── test_x_1.o
│   ├── test_x_2_mu.h
│   ├── test_x_2.o
│   └── x.o
├── LICENSE
├── Makefile
├── README.md
├── src
│   ├── minunit.h
│   ├── mu_header.sh
│   ├── tst
│   │   ├── test_x_1.c
│   │   └── test_x_2.c
│   ├── x.c
│   └── x.h
└── tst                    # unit test runners
    ├── test_x_1
    └── test_x_2

4 directories, 19 files
```

# Minunit

## test macros

- `mu_equal(a, b, fmt, ...)`  -- check equality, custom complaint message
- `mu_eq(a, b, fmt)` -- check equality, standard complaint message
- `mu_assert(exp)` -- check truth, standard complaint
- `mu_true(exp)` -- check truth, standard complaint
- `mu_false(exp)` -- check falsehood, standard complaint

## unit test files

A complete unit test file:

```c
// file: test_x_1.c -- a (minunit) test file for 'x.c'

#include "x.h"            // unit under test
#include "minunit.h"      // provides mu test macros & main function
#include "test_x_1_mu.h"  // provides test func declarations & the test runner

void test_xyz(void)
{
    int a=1, b=1;

    // these fail
    mu_assert(summ(a,b) == 3);
    mu_equal(summ(a,b), 3, "a+b really should equal %d!!", 3);
    mu_eq(3, summ(a,b), "%d");
    mu_true(summ(a,b) == 3);
    mu_false(summ(a,b) == 2);
}
```

The `main` for this unit test is in `minunit.h` while the autogenerated
`test_x_1_mu.h` file provides the test function declarations and a test runner
(called from `main`) to run all unit test functions.  The test functions, with
name/type of `void test_<..>(void)`, are run in the order of appearance in the
test file.

## unit test's mu-header file:

```c
// AUTOGENERATED bld/test_x_1_mu.h -> to '#include' in src/tst/test_x_1.c

// test functions

void test_xyz(void);

// test runner

void
mu_run_tests(void)
{
    mu_verify(test_xyz);
}
```

## test output

The test macros only complain on failures, like this:

```bash
% make test_x_1
==31477== Memcheck, a memory error detector
==31477== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==31477== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==31477== Command: ./tst/test_x_1
==31477== 
src/tst/test_x_1.c:12: test_xyz - assertion error 'summ(a,b) == 3'
src/tst/test_x_1.c:13: test_xyz - a+b really should equal 3!!
src/tst/test_x_1.c:14: test_xyz - expected 3, got 2
src/tst/test_x_1.c:15: test_xyz - expected 'summ(a,b) == 3' to be true
src/tst/test_x_1.c:16: test_xyz - expected 'summ(a,b) == 2' to be false
-------------------------------------
Ran 5 tests -> ok (0), fail (5)

==31477== 
==31477== HEAP SUMMARY:
==31477==     in use at exit: 0 bytes in 0 blocks
==31477==   total heap usage: 1 allocs, 1 frees, 1,024 bytes allocated
==31477== 
==31477== All heap blocks were freed -- no leaks are possible
==31477== 
==31477== For counts of detected and suppressed errors, rerun with: -v
==31477== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
Makefile:61: recipe for target 'test_x_1' failed
make: *** [test_x_1] Error 1
```

Error messages are formatted as: `file:line: <test_func_name> - msg`.

# Makefile

If valgrind is not available, either remove the `$(GRIND) $(VGOPT)` parts or
set those to be emtpy.

```Make
# run all unit tests (remove the ; to stop at first error)
test: $(MU_R)
	@$(foreach runner, $(MU_R), $(GRIND) $(VGOPT) ./$(runner);)

# run a single unit test
$(MU_T): %: $(TDIR)/%
	@$(GRIND) $(VGOPT) ./$<
```
