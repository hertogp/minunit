# minunit

A working example of a minimalistic unit testing 'framework', adapted from:
- [*minunit*](http://www.jera.com/techinfo/jtns/jtn003.html) by John Brewer, and
- [*tdd in c*](http://eradman.com/posts/tdd-in-c.html) by Eric Radman.

Files:
- `minunit.h` -- the _unit testing 'framework'_.
- `mu_header` -- a bash script to generate a unit test file's mu-header file.

and, as an example:
- `x.c`        -- the 'program' under test.
- `test_x_1.c` -- a unit test file for testing `x.c`.
- `test_x_2.c` -- a second unit test file for testing `x.c`.
- `Makefile`   -- to build and run the example code.

That's it.

To use it in a (simple) c-project, just:
- copy `minunit.h`,  `mu_header` to the new project
- write some unit tests in `test_<name>.c`
- update your `Makefile` with a test target
and away you go!


## Try it out

```bash
cd ~/topdir
git clone git@github.com:hertogp/minunit.git
cd minunit
make
```

Which should yield something along the lines of:

``` bash
% make
cc -Wall -pedantic -std=c99 -c x.c -o x.o
cc -Wall -pedantic -std=c99 x.o -o x
./mu_header test_x_1
cc -Wall -pedantic -std=c99 -c test_x_1.c -o test_x_1.o
strip -N main x.o -o x_stripped.o
cc -Wall -pedantic -std=c99 test_x_1.o x_stripped.o -o test_x_1
./mu_header test_x_2
cc -Wall -pedantic -std=c99 -c test_x_2.c -o test_x_2.o
strip -N main x.o -o x_stripped.o
cc -Wall -pedantic -std=c99 test_x_2.o x_stripped.o -o test_x_2
test_x_1.c:12: test_xyz - assertion error 'summ(a,b) == 3'
test_x_1.c:13: test_xyz - a+b really should equal 3!!
test_x_1.c:14: test_xyz - expected 3, got 2
test_x_1.c:15: test_xyz - expected 'summ(a,b) == 3' to be true
test_x_1.c:16: test_xyz - expected 'summ(a,b) == 2' to be false
-------------------------------------
Ran 10 tests -> ok (5), fail (5)

test_x_2.c:13: test_summ_int - expected 42, got 43
test_x_2.c:14: test_summ_int - assertion error '42 == answer'
test_x_2.c:15: test_summ_int - expected '42 == answer' to be true
test_x_2.c:22: test_summ_float - expected 4.2, got 4.000000 instead
test_x_2.c:23: test_summ_float - expected 4.2, got 4.0
test_x_2.c:32: test_summ_mix - expected 41, got 42
-------------------------------------
Ran 7 tests -> ok (1), fail (6)

Makefile:35: recipe for target 'test' failed
make: *** [test] Error 1
```

Have a look at what `minunit.h` and `test_x_mu.h` do:

```bash
gcc -E test_x.c
```

## unit tests

The following is a complete unit test file with 1 test function, using
all mu test macros..

```c
// file: test_x_1.c -- a (minunit) test file for 'x.c'

#include "x.h"            // unit under test
#include "minunit.h"      // provides mu test macros
#include "test_x_1_mu.h"  // provides main and the test runner

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

// Note: no main here (see test_x_mu.h)
// - just write the test_functions ...
```

running just the `test_x_1` unit test, yields:

```bash
$ make just_test_x_1

./test_x_1
test_x_1.c:12: test_xyz - assertion error 'summ(a,b) == 3'
test_x_1.c:13: test_xyz - a+b really should equal 3!!
test_x_1.c:14: test_xyz - expected 3, got 2
test_x_1.c:15: test_xyz - expected 'summ(a,b) == 3' to be true
test_x_1.c:16: test_xyz - expected 'summ(a,b) == 2' to be false
-------------------------------------
Ran 10 tests -> ok (5), fail (5)

Makefile:39: recipe for target 'just_test_x_1' failed
make: [just_test_x_1] Error 1 (ignored)
```

## Makefile

The Makefile collects all test runners names based on the `test_<name>.c`
naming convention.  Running just `make` will run all test runners, running
a single test is done by prepending 'just\_' to the test runners' name, e.g.
`make just_test_x_1`.


```make
# The test runners are named 'test_<name>.c'

test_cfiles=$(sort $(wildcard test_*.c))
test_ofiles=$(test_cfiles:.c=.o)
test_hfiles=$(test_cfiles:.c=_mu.h)
test_runners=$(test_cfiles:.c=)

# run *all* test runners
# - remove the ';' to stop make'ing after a test runner fails

test: $(test_runners)
	@$(foreach test, $(test_runners), ./$(test);)

# `make just_test_<name>', runs a single test_<name>
# - a bit clumsy

$(test_runners:%=just_%): just_%: %
	-./$<
```

The `minunit.h` contains a boiler plate `main()` that simply calls a standard
`mu_run_tests()`.  In order to avoid multiple mains, when building a test
runner, a stripped version of TEST is used:

```make
# generate a test runners' test_<name>_mu.h header file
# - this contains generated mu_run_tests() function for the test runner

$(test_hfiles): %_mu.h: %.c
	./mu_header $*

# strip main from x, the test runner provides 'main'

$(test_runners): %: minunit.h %_mu.h %.o ${TEST}.o
	strip -N main ${TEST}.o -o ${TEST}_stripped.o
	${CC} ${CFLAGS} $@.o ${TEST}_stripped.o -o $@
```

## `mu_header`

`mu_header` collects all `void test_<name>(void)` test functions and creates
a `test_<name>_mu.h` header file, containing:
- all test function declarations, and
- the `mu_run_tests()` containing the calls to each test function

A test function signature is `void test_xyz(void)`.  The macro's used in the
test function keep track of succes/failure's.  A test runner returns failure
if 1 or more (mu macro) tests failed.
