#!/bin/bash
make clean
rm -rf src/build
make coveragetest
cd src/

lcov -t "result" -o coverage_out.info -c -d ./build/;
lcov --remove coverage_out.info "/usr/include/*" -o coverage_out2.info;
lcov --remove coverage_out2.info "*.h" -o coverage_out3.info;
sed "s/SF:.*\/HiPPSO\/src\//SF:/" coverage_out3.info > coverage_out4.info;
lcov --list coverage_out4.info;
genhtml -o res coverage_out4.info

firefox res/index.html &
cd ..
