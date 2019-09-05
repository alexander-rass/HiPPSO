#!/bin/bash

checkFilesMatching(){
    reffile="$1"
    prodfile="$2"

    if [ "${prodfile: -7}" == ".backup" ]; then
        if [ "${reffile: -7}" == ".backup" ]; then
            prodlines=$(cat "$prodfile" | wc -l)
            reflines=$(cat "$reffile" | wc -l)
            if [ $prodlines -ne $reflines ] ; then
                return 1
            fi
            prodwords=$(cat "$prodfile" | wc -w)
            refwords=$(cat "$reffile" | wc -w)
            if [ $prodwords -ne $refwords ] ; then
                return 1
            fi
            cmp --silent <(sed "${prodlines}d" "$prodfile" | sed "1d") <(sed "${reflines}d" "$reffile" | sed "1d") || return 1
            #check that first line contains only a single string which represents the version
            expectedoneword=$(head -n 1 "$prodfile" | wc -w)
            if [ "$expectedoneword" -ne 1 ]; then
                return 1
            fi
            expectedoneword=$(head -n 1 "$reffile" | wc -w)
            if [ "$expectedoneword" -ne 1 ]; then
                return 1
            fi
        else
            return 1
        fi
    elif [ "${prodfile: -4}" == ".log" ]; then
        if [ "${reffile: -4}" == ".log" ] ; then
            return 0
        else
            return 1
        fi
    elif [ "${prodfile: -12}" == ".gnuplot.txt" ]; then
        if [ "${reffile: -12}" == ".gnuplot.txt" ] ; then
            cmp --silent <(sed "s/set xrange \[.*\]/set xrange []/" "$prodfile" | sed "s/set yrange \[.*\]/set yrange []/" | sed "s/set output '0*/set output '/" | grep -v "nohead") <(sed "s/set xrange \[.*\]/set xrange []/" "$reffile" | sed "s/set yrange \[.*\]/set yrange []/" | sed "s/set output '0*/set output '/" | grep -v "nohead") || return 1
        else
            return 1
        fi
    else
        cmp --silent "$reffile" "$prodfile" || return 1
    fi
    return 0
}

checkFoldersMatching(){
    REFFOLDER="$1"
    PRODFOLDER="$2"

    #store filenames of each folder in an array:
    PRODUCEDFILEARRAY=()
    while IFS=  read -r -d $'\0'; do
        PRODUCEDFILEARRAY+=("$REPLY")
    done < <(find "$PRODFOLDER" -type f -print0)
    REFFILEARRAY=()
    while IFS=  read -r -d $'\0'; do
        REFFILEARRAY+=("$REPLY")
    done < <(find "$REFFOLDER" -type f -print0)

    #check whether number of files is equal
    if [ ${#REFFILEARRAY[@]} -ne ${#PRODUCEDFILEARRAY[@]} ]; then
        echo "FAILURE DETECTED"
        echo "Number of prduced files (${#PRODUCEDFILEARRAY[@]}) differs from number of reference files (${#REFFILEARRAY[@]})"
        return 1
    fi

    for prodindex in ${!PRODUCEDFILEARRAY[@]}; do
        sameindex=-1
        for refindex in ${!REFFILEARRAY[@]}; do
            reffile="${REFFILEARRAY[$refindex]}"
            prodfile="${PRODUCEDFILEARRAY[$prodindex]}"
            checkFilesMatching "$reffile" "$prodfile" && sameindex=$refindex && break
        done
        if [ $sameindex -ne -1 ]; then
            unset REFFILEARRAY[$refindex]
        else
            echo "No match for file ${PRODUCEDFILEARRAY[$prodindex]}"
            return 1
        fi
    done
    return 0
}

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
