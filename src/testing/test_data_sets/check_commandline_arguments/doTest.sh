#!/bin/bash

source ../compareFolders.sh


setsid ./createDataRestartAll.sh testfolder

EXITCODE=0
for TESTID in $(seq 1 3); do
	checkFoldersMatching "testfolder/stage$TESTID" "reference_data/stage$TESTID"  || EXITCODE=1
	if [ $EXITCODE -eq 1 ]; then
		exit 1;
	fi
done
rm -r testfolder
exit $EXITCODE

