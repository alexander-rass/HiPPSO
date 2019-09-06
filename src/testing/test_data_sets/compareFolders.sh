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

    if [ ! -d $REFFOLDER ]; then
        echo "$REFFOLDER does not exist";
        return 1
    fi
    if [ ! -d $PRODFOLDER ]; then
        echo "$PRODFOLDER does not exist";
        return 1
    fi
    #store filenames of each folder in an array:
    PRODUCEDFILEARRAY=()
    while IFS=  read -r -d $'\0'; do
        if [ "${REPLY: -10}" != ".stackdump" ]; then
            PRODUCEDFILEARRAY+=("$REPLY")
        fi
    done < <(find "$PRODFOLDER" -type f -print0)
    REFFILEARRAY=()
    while IFS=  read -r -d $'\0'; do
        if [ "${REPLY: -10}" != ".stackdump" ]; then
            REFFILEARRAY+=("$REPLY")
        fi
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

