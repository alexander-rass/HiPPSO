#!/bin/bash

FOLDER="$1"
if [ -d "$FOLDER" ]; then
    rm -r "$FOLDER";
fi

mkdir "$FOLDER"
cd "$FOLDER"
mkdir stage1
cd stage1
cp ../../configuration_files/runcheck_allowed.conf runcheck.conf
../../../../../../bin/high_precision_pso c ../../configuration_files/configuration_file1.conf
../../../../../../bin/high_precision_pso c ../../configuration_files/configuration_file2.conf
cd ..
cp -R stage1 stage2
cd stage2
cp ../../configuration_files/runcheck_forbidden.conf runcheck.conf
for TESTID in $(seq 1 2); do
    cp test$TESTID.confBU tmp.confBU
    sed "s/steps 100/steps 200/" tmp.confBU > test$TESTID.confBU
    rm tmp.confBU
    touch test$TESTID.SHUTDOWN
done
cp -R . ../stage3
../../../../../../bin/high_precision_pso restartAll . > /dev/null 2> /dev/null
cd ..
cp ../configuration_files/runcheck_allowed.conf stage3/runcheck.conf
../../../../../bin/high_precision_pso restartAll stage3 > /dev/null 2> /dev/null

mkdir stage4
cd stage4
../../../../../../bin/high_precision_pso restart ../stage2/test1.confBU ../stage2/test1.S50.backup

cd ../..

# wait a short time to let subprocesses be finished
sleep 3
