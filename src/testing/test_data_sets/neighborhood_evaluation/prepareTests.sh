#!/bin/bash

ITERATIONS="142"

for TESTID in 1 2 ; do
    echo Testseries $TESTID
    REFFOLDER="reference_data$TESTID"
    NEIGHBORHOODOPTIONS="neighborhood_options$TESTID.conf"
    # create clean folder for reference data
    if [ -d $REFFOLDER ] ; then
        rm -r $REFFOLDER
    fi
    mkdir $REFFOLDER

    numoptions=$(cat $NEIGHBORHOODOPTIONS | wc -l);
    for line in $(seq 1 $numoptions) ;
    do
        tmpconffile=tmp.conf ;
        cat base_configfile$TESTID.conf > $tmpconffile;
        optionDescription=$(head -n$line $NEIGHBORHOODOPTIONS | tail -n1);
        echo "neighborhood $optionDescription" >> $tmpconffile;
        echo "steps $ITERATIONS" >> $tmpconffile
        folder=run$line ;
        mkdir $REFFOLDER/$folder
        cd $REFFOLDER/$folder
        ../../../../../../bin/high_precision_pso c ../../$tmpconffile ;
        cd ../..
        rm $tmpconffile
    done
done
