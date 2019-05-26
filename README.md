# minunit

A working example of a minimalistic C unit testing 'framework', adapted from:
- [*minunit*](http://www.jera.com/techinfo/jtns/jtn003.html) by John Brewer, and
- [*tdd in c*](http://eradman.com/posts/tdd-in-c.html) by Eric Radman.

Project setup:
```bash
.
├── bld                   # build dir
├── LICENSE
├── Makefile              # assumes valgrind is installed
├── README.md
├── src
│   ├── minunit.h         # minunit test macros
│   ├── mu_header.sh      # autogenerates unit test file's _mu.h header
│   ├── tst               # contains the unit test files
│   │   ├── test_x_1.c
│   │   └── test_x_2.c
│   ├── x.c               # code to be tested
│   └── x.h
└── tst                   # contains the unit test runners

5 directories, 9 files
```

Running `make`, compiles the `src/*.c` files into `bld/libx.so.1.0.1`, with a
`-soname=libx.so.1`.  Running `make test` runs all unit tests, compiling the
`src/tst/test_*.c` files into `tst/test_<t>` executables when needed.

Individual unit tests targets are derived from the unit test c-files, so `make
test_x_1` would run just that unit test. Finally `make clean` clears both the
`bld` and `tst` subdirectories.

Here's the tree after `make test`:

```bash
.
├── bld
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
├── scr
├── src
│   ├── minunit.h
│   ├── mu_header.sh
│   ├── tst
│   │   ├── test_x_1.c
│   │   └── test_x_2.c
│   ├── x.c
│   └── x.h
└── tst
    ├── test_x_1
    └── test_x_2

5 directories, 19 files
```

Minunit test macros only complain on failures, which looks like the following:

```bash
% make test
src/mu_header.sh src/tst/test_x_1.c bld/test_x_1_mu.h

cc -Ibld -Isrc -std=c99 -O2 -g -Wall -Wextra -Werror -pedantic -fPIC
-D_POSIX_C_SOURCE=200809L -D_GNU_SOURCE -D_DEFAULT_SOURCE
-Wno-unknown-warning-option -Wold-style-definition -Wstrict-prototypes
-Wmissing-prototypes -Wpointer-arith -Wmissing-declarations -Wredundant-decls
-Wnested-externs -Wshadow -Wcast-qual -Wcast-align -Wwrite-strings
-Wsuggest-attribute=noreturn -Wjump-misses-init -o bld/test_x_1.o -c
src/tst/test_x_1.c

cc -std=c99 -O2 -g -Wall -Wextra -Werror -pedantic -fPIC
-D_POSIX_C_SOURCE=200809L -D_GNU_SOURCE -D_DEFAULT_SOURCE
-Wno-unknown-warning-option -Wold-style-definition -Wstrict-prototypes
-Wmissing-prototypes -Wpointer-arith -Wmissing-declarations -Wredundant-decls
-Wnested-externs -Wshadow -Wcast-qual -Wcast-align -Wwrite-strings
-Wsuggest-attribute=noreturn -Wjump-misses-init -c src/x.c -o bld/x.o

cc -fPIC -shared -Wl,-soname=libx.so.1 bld/x.o -o bld/libx.so.1.0.1

cc -Lbld -Wl,-rpath,.:bld bld/test_x_1.o -o tst/test_x_1 -lx

src/mu_header.sh src/tst/test_x_2.c bld/test_x_2_mu.h

cc -Ibld -Isrc -std=c99 -O2 -g -Wall -Wextra -Werror -pedantic -fPIC
-D_POSIX_C_SOURCE=200809L -D_GNU_SOURCE -D_DEFAULT_SOURCE
-Wno-unknown-warning-option -Wold-style-definition -Wstrict-prototypes
-Wmissing-prototypes -Wpointer-arith -Wmissing-declarations -Wredundant-decls
-Wnested-externs -Wshadow -Wcast-qual -Wcast-align -Wwrite-strings
-Wsuggest-attribute=noreturn -Wjump-misses-init -o bld/test_x_2.o -c
src/tst/test_x_2.c

cc -Lbld -Wl,-rpath,.:bld bld/test_x_2.o -o tst/test_x_2 -lx

==30644== Memcheck, a memory error detector
==30644== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==30644== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==30644== Command: ./tst/test_x_1
==30644== 
src/tst/test_x_1.c:12: test_xyz - assertion error 'summ(a,b) == 3'
src/tst/test_x_1.c:13: test_xyz - a+b really should equal 3!!
src/tst/test_x_1.c:14: test_xyz - expected 3, got 2
src/tst/test_x_1.c:15: test_xyz - expected 'summ(a,b) == 3' to be true
src/tst/test_x_1.c:16: test_xyz - expected 'summ(a,b) == 2' to be false
-------------------------------------
Ran 10 tests -> ok (5), fail (5)

==30644== 
==30644== HEAP SUMMARY:
==30644==     in use at exit: 0 bytes in 0 blocks
==30644==   total heap usage: 1 allocs, 1 frees, 1,024 bytes allocated
==30644== 
==30644== All heap blocks were freed -- no leaks are possible
==30644== 
==30644== For counts of detected and suppressed errors, rerun with: -v
==30644== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

==30645== Memcheck, a memory error detector
==30645== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==30645== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==30645== Command: ./tst/test_x_2
==30645== 
src/tst/test_x_2.c:13: test_summ_int - expected 42, got 43
src/tst/test_x_2.c:14: test_summ_int - assertion error '42 == answer'
src/tst/test_x_2.c:15: test_summ_int - expected '42 == answer' to be true
src/tst/test_x_2.c:22: test_summ_float - expected 4.2, got 4.000000 instead
src/tst/test_x_2.c:23: test_summ_float - expected 4.2, got 4.0
src/tst/test_x_2.c:32: test_summ_mix - expected 41, got 42
-------------------------------------
Ran 7 tests -> ok (1), fail (6)

==30645== 
==30645== HEAP SUMMARY:
==30645==     in use at exit: 0 bytes in 0 blocks
==30645==   total heap usage: 1 allocs, 1 frees, 1,024 bytes allocated
==30645== 
==30645== All heap blocks were freed -- no leaks are possible
==30645== 
==30645== For counts of detected and suppressed errors, rerun with: -v
==30645== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

Makefile:57: recipe for target 'test' failed
make: *** [test] Error 1
```
