# minunit

A working example of [*minunit*](http://www.jera.com/techinfo/jtns/jtn003.html)
, a minimalist unit testing 'framework' for C programs.  Slightly modified but
still as simple as possible.

Files:
- minunit.h, the _unit testing 'framework'_.
- mu\_header, bash script to generate a unit test file's mu-header file.
- x.c, the 'program' under test.
- test\_x.c, the unit test file for testing x.c.
- Makefile, to build and run the example code.

That's it.

To use it in a (simple) c-project, just:
- copy minunit.h, mu\_header to the new project
- write some unit tests in test\_\<name>.c
- update your Makefile with a test target
and away you go!


## Try it out

```bash
cd ~/topdir
git clone git@github.com:hertogp/minunit.git
cd minunit
make
```
You should see something along the lines of:
``` bash
cc -Wall -pedantic -std=c99   -c -o x.o x.c
cc -Wall -pedantic -std=c99 x.o -o x
./mu_header test_x
cc -Wall -pedantic -std=c99   -c -o test_x.o test_x.c
strip -N main x.o -o x_stripped.o
cc -Wall -pedantic -std=c99 test_x.o x_stripped.o -o test_x
./test_x
test_x.c:20 (test_xyz) 1+1 really should equal 2
test_x.c:21 (test_xyz) assertion error '1+1 == 3'
test_x.c:22 (test_xyz) expected '1+1 == 3' to be true
test_x.c:23 (test_xyz) expected '1+1 == 2' to be false
test_x.c:32 (test_summ_int) expected 42, got 43
test_x.c:33 (test_summ_int) assertion error '42 == answer'
test_x.c:34 (test_summ_int) expected '42 == answer' to be true
test_x.c:41 (test_summ_float) expected 4.2, got 4.000000 instead
test_x.c:42 (test_summ_float) expected 4.2, got 4.0
test_x.c:51 (test_summ_mix) expected 41, got 42
-------------------------------------
Ran 15 tests -> ok (5), fail (10)
Makefile:21: recipe for target 'test' failed
make: *** [test] Error 1
```

