#!/bin/bash
make clean
rm -rf src/build
make coveragetest
cd src/build
lcov -t "result" -o ex_text.info -c -d .
genhtml -o res ex_text.info

firefox res/index.html &
cd ../..
