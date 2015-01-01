#!/bin/sh
basepath=$1

make
lcov --zerocounters --directory .
ctest
lcov --directory . --gcov-tool ${basepath}/llvm-gcov.sh --capture -o cov.info
genhtml --output-directory coverage \
  --demangle-cpp --num-spaces 2 --sort \
  --title "dyabl" \
  --function-coverage --branch-coverage --legend \
  cov.info

