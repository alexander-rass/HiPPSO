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

./createDataRestartAll.sh testfolder

EXITCODE=0
for TESTID in $(seq 1 3); do
	checkFoldersMatching "testfolder/stage$TESTID" "reference_data/stage$TESTID"  || EXITCODE=1
	if [ $EXITCODE -eq 1 ]; then
		cd $TESTBASEFOLDER
		exit 1;
	fi
done
rm -r testfolder

../../../../bin/high_precision_pso -version > version.txt
VERSIONLINES=$(cat version.txt | wc -l)
if [ $VERSIONLINES -ne 1 ]; then
	EXITCODE=1
else
	VERSIONWORDS=$(cat version.txt | wc -w)
	if [ $VERSIONWORDS -ne 2 ]; then
		EXITCODE=1
	else
		filecontent="$(cat version.txt)"
		if [ "${filecontent:0:9}" != "version: " ]; then
			EXITCODE=1
		fi
	fi
fi

../../../../bin/high_precision_pso -gmpversion > version.txt
VERSIONLINES=$(cat version.txt | wc -l)
if [ $VERSIONLINES -ne 1 ]; then
	EXITCODE=1
else
	VERSIONWORDS=$(cat version.txt | wc -w)
	if [ $VERSIONWORDS -ne 3 ]; then
		EXITCODE=1
	else
		filecontent="$(cat version.txt)"
		if [ "${filecontent:0:13}" != "gmp version: " ]; then
			EXITCODE=1
		fi
	fi
fi
rm version.txt
mkdir testfolder
../../../../bin/high_precision_pso > testfolder/usage.txt
checkFoldersMatching "testfolder" "reference_data/usage"  || EXITCODE=1
rm -r testfolder


if [ $EXITCODE -eq 0 ] ; then
    echo "Test succeeded."
else
    echo "TEST FAILED!!"
fi
exit $EXITCODE

