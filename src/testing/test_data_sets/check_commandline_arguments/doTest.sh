#!/bin/bash

source ../compareFolders.sh

REFFOLDER=reference_data
TESTBASEFOLDER=$(pwd)

if [ -d $REFFOLDER ] ; then
    echo "Start test.";
else
    echo "Missing REFFOLDER data.";
    exit 1;
fi

setsid ./createDataRestartAll.sh testfolder

EXITCODE=0
for TESTID in $(seq 1 3); do
	checkFoldersMatching "testfolder/stage$TESTID" "reference_data/stage$TESTID"  || EXITCODE=1
	if [ $EXITCODE -eq 1 ]; then
		cd $TESTBASEFOLDER
		exit 1;
	fi
done
rm -r testfolder

if [ $EXITCODE -eq 0 ] ; then
    echo "Test succeeded."
else
    echo "TEST FAILED!!"
fi
exit $EXITCODE

