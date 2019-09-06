#!/bin/bash

source ../compareFolders.sh

ITERATIONS=$3
SPLITITERATIONS=$2
REFFOLDER="$1"

TESTBASEFOLDER=$(pwd)

if [ -d $REFFOLDER ] ; then
    echo "Start test.";
else
    echo "Missing REFFOLDER data.";
    exit 1;
fi

EXITCODE=0
fileCount=$(ls | wc -l)

for folder in $REFFOLDER/* ; do
    if [ -d $folder ]; then
        echo "Test: $TESTBASEFOLDER/$folder"
        NUMCONFFILES=$(ls -1 $TESTBASEFOLDER/$folder/*.confBU | wc -l)
        if [ "$NUMCONFFILES" -ne 1 ] ; then
            echo "FAILUERE DETECTED - TEST STRUCTURE FAILED"
            EXITCODE=1
            exit 1
        fi
        CURCONFIGFILE=$TESTBASEFOLDER/$folder/*.confBU

        TMPFOLDER=tmpfolder
        if [ -d $TMPFOLDER ]; then
            rm -r $TMPFOLDER;
        fi
        TMPCONFIGFILE=tmpconffile.conf
        mkdir $TMPFOLDER
        cd $TMPFOLDER
        sed "s/^steps $ITERATIONS/steps $SPLITITERATIONS/" $CURCONFIGFILE > $TMPCONFIGFILE
        ../../../../../bin/high_precision_pso c $TMPCONFIGFILE > stdout.txt 2> stderr.txt;
        receivedExitCode=$? ;
        expectedExitCode=0;
        if [ $receivedExitCode -ne $expectedExitCode ] ; then
            echo "FAILURE DETECTED"
            echo "Exit code of the program was not as expected for $file."
            echo "Expected: $expectedExitCode"
            echo "Received: $receivedExitCode"
            EXITCODE=1
            cd $TESTBASEFOLDER
            exit 1
        fi
        rm $TMPCONFIGFILE
        NUMCONFFILES=$(ls -1 *.confBU | wc -l)
        if [ "$NUMCONFFILES" -ne 1 ] ; then
            echo "FAILUERE DETECTED"
            echo "Number of confBU files should be 1 but is $NUMCONFFILES"
            EXITCODE=1
            cd $TESTBASEFOLDER
            exit 1
        fi
        CURCONFIGFILE=$(echo *.confBU)
        sed "s/^steps $SPLITITERATIONS/steps $ITERATIONS/" $CURCONFIGFILE > TMP_$CURCONFIGFILE
        mv TMP_$CURCONFIGFILE $CURCONFIGFILE
        ../../../../../bin/high_precision_pso rf $CURCONFIGFILE >> stdout.txt 2>> stderr.txt;
        receivedExitCode=$? ;
        expectedExitCode=0;
        if [ $receivedExitCode -ne $expectedExitCode ] ; then
            echo "FAILURE DETECTED"
            echo "Exit code of the program was not as expected for $file."
            echo "Expected: $expectedExitCode"
            echo "Received: $receivedExitCode"
            EXITCODE=1
            cd $TESTBASEFOLDER
            exit 1
        fi
        STDOUTLINES=$(cat stdout.txt | wc -l)
        if [ $STDOUTLINES -eq 0 ]; then
            rm stdout.txt;
        fi
        STDERRLINES=$(cat stderr.txt | wc -l)
        if [ $STDERRLINES -eq 0 ]; then
            rm stderr.txt;
        fi
        checkFoldersMatching "$TESTBASEFOLDER/$folder" "."  || EXITCODE=1
        cd $TESTBASEFOLDER
        if [ $EXITCODE -eq 1 ]; then
            exit 1
        fi
        rm -r $TMPFOLDER
    else
        EXITCODE=1
        exit 1
    fi
done
if [ $EXITCODE -eq 0 ] ; then
    echo "Test succeeded."
else
    echo "TEST FAILED!!"
fi

exit $EXITCODE
