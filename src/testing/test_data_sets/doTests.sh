#!/bin/bash

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
        ../../../../../bin/high_precision_pso c $TMPCONFIGFILE;
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
        ../../../../../bin/high_precision_pso rf $CURCONFIGFILE;
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
        NUMREFFILES=$(find $TESTBASEFOLDER/$folder -type f -print0 | tr -d -c '\0' | wc -c)
        NUMPRODUCEDFILES=$(find . -type f -print0 | tr -d -c '\0' | wc -c)
        if [ $NUMREFFILES -ne $NUMPRODUCEDFILES ]; then
            echo "FAILURE DETECTED"
            echo "Number of prduced files ($NUMPRODUCEDFILES) differs from number of reference files ($NUMREFFILES)"
            EXITCODE=1
            cd $TESTBASEFOLDER
            exit 1
        fi
        find . -type f -print0 |
            while IFS= read -r -d '' line; do
                currentfile="$line"
                reffile=$(echo "$currentfile" | sed "s|\./|$TESTBASEFOLDER/$folder/|" | sed "s/S${SPLITITERATIONS}F/S${ITERATIONS}F/")
                if [ -f $reffile ]; then
                    if [ ${currentfile: -7} == ".backup" ]; then
                        #compare all except first and last line (those two lines contain the version number)
                        differLines=$(diff <(sed "$(cat $currentfile | wc -l)d" $currentfile | sed "1d") <(sed "$(cat $reffile | wc -l)d" $reffile | sed "1d") | wc -l)
                        #check that first line contains only a single string which represents the version
                        expectedoneword=$(head -n 1 $currentfile | wc -w)
                        if [ "$expectedoneword" -ne 1 ]; then
                            differLines=1
                        fi
                    elif [ ${currentfile: -4} == ".log" ]; then
                        differLines=0
                    else
                        differLines=$(diff $currentfile $reffile | wc -l)
                    fi
                    if [ $differLines -ne 0 ] ; then
                        echo "FAILURE DETECTED"
                        echo "$reffile does not match!"
                        EXITCODE=1
                        cd $TESTBASEFOLDER
                        exit 1
                    fi
                else
                    echo "FAILURE DETECTED"
                    echo "No reference file for data file $currentfile found."
                    EXITCODE=1
                    cd $TESTBASEFOLDER
                    exit 1
                fi
            done
        receivedExitCode=$? ;
        if [ $receivedExitCode -ne 0 ] ; then
            echo "FAILURE DETECTED"
            echo "failure appeared while testing $folder"
            EXITCODE=1
            cd $TESTBASEFOLDER
            exit 1
        fi
        cd $TESTBASEFOLDER
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
