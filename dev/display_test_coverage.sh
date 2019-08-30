#!/bin/bash
GCOV_FOLDER_PREFIX="$(pwd)/src/gcovdata"
GCOV_FOLDER_PREFIX_STRIP=$(echo "${GCOV_FOLDER_PREFIX}" | awk -F"/" '{print NF-1}')
export GCOV_PREFIX_STRIP=${GCOV_FOLDER_PREFIX_STRIP}
export GCOV_PREFIX=${GCOV_FOLDER_PREFIX}
#echo $GCOV_PREFIX
#echo $GCOV_PREFIX_STRIP
make clean
rm -rf src/gcovdata
mkdir src/gcovdata
make coveragetest

for file in $(find . -name "*.gcno"); do 
    ofile=$(echo $file | sed "s/build/gcovdata/");
    echo Copy $file to $ofile ;
    if [ -f $ofile ]; then
        echo $ofile DONE;
    else
        cp $file $ofile;
    fi;
done

cd src
rm -f ex_text.info
lcov -t "result" -o ex_text.info -c -d .
rm -rf res
genhtml -o res ex_text.info

firefox res/index.html &
cd ..
