# minunit

A working example of a minimalistic unit testing 'framework', adapted from:
- [*minunit*](http://www.jera.com/techinfo/jtns/jtn003.html) by John Brewer, and
- [*tdd in c*](http://eradman.com/posts/tdd-in-c.html) by Eric Radman.

Files:
- `minunit.h` -- the _unit testing 'framework'_.
- `mu_header` -- a bash script to generate a unit test file's mu-header file.

and, as an example:
- `x.c`      -- the 'program' under test.
- `test_x.c` -- the unit test file for testing `x.c`.
- `Makefile` -- to build and run the example code.

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
cc -Wall -pedantic -std=c99   -c -o x.o x.c
cc -Wall -pedantic -std=c99 x.o -o x
./mu_header test_x
cc -Wall -pedantic -std=c99   -c -o test_x.o test_x.c
strip -N main x.o -o x_stripped.o
cc -Wall -pedantic -std=c99 test_x.o x_stripped.o -o test_x
./test_x
test_x.c:25: test_xyz - assertion error 'summ(a,b) == 3'
test_x.c:26: test_xyz - a+b really should equal 3!!
test_x.c:27: test_xyz - expected 2, got 3
test_x.c:28: test_xyz - expected 'summ(a,b) == 3' to be true
test_x.c:29: test_xyz - expected 'summ(a,b) == 2' to be false
test_x.c:38: test_summ_int - expected 42, got 43
test_x.c:39: test_summ_int - assertion error '42 == answer'
test_x.c:40: test_summ_int - expected '42 == answer' to be true
test_x.c:47: test_summ_float - expected 4.2, got 4.000000 instead
test_x.c:48: test_summ_float - expected 4.2, got 4.0
test_x.c:57: test_summ_mix - expected 41, got 42
-------------------------------------
Ran 17 tests -> ok (6), fail (11)
Makefile:21: recipe for target 'test' failed
make: *** [test] Error 1
```

The `test_x.c` unit test file contains some checks that succeed and some that
fail.  Have a look at what `minunit.h` and `test_x_mu.h` do:

```bash
gcc -E test_x.c
```

## unit tests

The following is a complete unit test file with 1 test function, using
all mu test macros..

```c
// file test_x.c

#include "x.h"              // unit under test (provides summ)
#include "minunit.h"        // provides mu test macros
#include "test_x_mu.h"      // provides main and the test runner

void test_xyz(void)
{
    int a=1, b=1;

    // these all fail
    mu_assert(summ(a,b) == 3);
    mu_equal(summ(a,b), 3, "a+b really should equal %d!!", 3);
    mu_eq(3, summ(a,b), "%d");
    mu_true(summ(a,b) == 3);
    mu_false(summ(a,b) == 2);
}

// other tests ...

// Note: no main here (see test_x_mu.h)
```

running this yields:

```bash
$ make
./test_x
test_x.c:12: test_xyz - assertion error 'summ(a,b) == 3'
test_x.c:13: test_xyz - a+b really should equal 3!!
test_x.c:14: test_xyz - expected 3, got 2
test_x.c:15: test_xyz - expected 'summ(a,b) == 3' to be true
test_x.c:16: test_xyz - expected 'summ(a,b) == 2' to be false
<snip other tests>
-------------------------------------
Ran 17 tests -> ok (6), fail (11)
Makefile:26: recipe for target 'test' failed
make: *** [test] Error 1
```

## Makefile

The Makefile runs `mu_header` to create `test_x_mu.h` for the unit test file
`test_x.c` and, because `x.c` contains a `main`, it also creates a stripped
version of `x.o` with main removed since that's provided by the header
generated earlier.

```make
# generate test_x_mu.h file, to be #include'd in 'test_x.c'
test_x_mu.h: test_x.c
	./mu_header test_x

# strip main from x, the test runner provides 'main'
test_x: minunit.h test_x_mu.h test_x.o x.o
	strip -N main x.o -o x_stripped.o
	${CC} ${CFLAGS} test_x.o x_stripped.o -o $@
```
