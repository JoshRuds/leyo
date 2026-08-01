#!/bin/sh

make clean >/dev/null 2>&1
make >/dev/null 2>&1 || exit 1

./bin/leyo build tests/recursion.leyo >/dev/null 2>&1 || exit 1

exit 0