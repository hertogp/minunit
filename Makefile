
# TEST is the unit under test

TEST=x

CFLAGS=-Wall -pedantic -std=c99
LDFLAGS= -static ${LIBS}

# The test runners are named 'test_<name>.c'

test_cfiles=$(sort $(wildcard test_*.c))
test_ofiles=$(test_cfiles:.c=.o)
test_hfiles=$(test_cfiles:.c=_mu.h)
test_runners=$(test_cfiles:.c=)

# builds TEST and runs 'test' (all runners)

all: ${TEST} test

.PHONY: all clean

install:
	@echo "no installation required nor needed"

# Remove test runners, autogenerated header files, and executables

clean:
	-rm -f *_mu.h *.o $(test_runners) ${TEST}

# The libary under TEST

${TEST}.o: ${TEST}.h

${TEST}: ${TEST}.o
	$(CC) ${CFLAGS} $< -o $@

# Explicit so CFLAGS are actually used

.c.o:
	$(CC) ${CFLAGS} -c $< -o $@


# run *all* test runners
# - remove the ';' to stop make'ing after a test runner fails

test: $(test_runners)
	@$(foreach test, $(test_runners), ./$(test);)


# `make just_test_<name>', runs a single test_<name>
# - a bit clumsy

$(test_runners:%=just_%): just_%: %
	-./$<


# generate a test runners' test_<name>_mu.h header file
# - this contains generated mu_run_tests() function for the test runner

$(test_hfiles): %_mu.h: %.c
	./mu_header $*


# strip main from x, the test runner provides 'main'

$(test_runners): %: minunit.h %_mu.h %.o ${TEST}.o
	strip -N main ${TEST}.o -o ${TEST}_stripped.o
	${CC} ${CFLAGS} $@.o ${TEST}_stripped.o -o $@
