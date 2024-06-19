.PHONY: all clean test

all: source test

source:
		$(MAKE) -C src

test:
		$(MAKE) -C tests

run_tests:
		./tests/build/tests/run

clean:
		$(MAKE) -C src clean
		$(MAKE) -C tests clean
