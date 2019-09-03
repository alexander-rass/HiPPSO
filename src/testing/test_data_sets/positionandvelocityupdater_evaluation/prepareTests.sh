#!/bin/bash

TESTSETS=$1
ITERATIONS="142"

for TESTID in $(seq 1 $TESTSETS) ; do
    echo Testseries $TESTID
    REFFOLDER="reference_data$TESTID"
    SPECIALOPTIONS="special_options/special_options$TESTID.conf"
    # create clean folder for reference data
    if [ -d $REFFOLDER ] ; then
        rm -r $REFFOLDER
    fi
    mkdir $REFFOLDER

    numoptions=$(cat $SPECIALOPTIONS | wc -l);
    for line in $(seq 1 $numoptions) ;
    do
        tmpconffile=tmp.conf ;
        cp base_configfiles/base_configfile$TESTID.conf $tmpconffile;
        optionDescription=$(head -n$line $SPECIALOPTIONS | tail -n1);
        echo "$optionDescription" >> $tmpconffile;
        echo "steps $ITERATIONS" >> $tmpconffile
        folder=run$line ;
        mkdir $REFFOLDER/$folder
        cd $REFFOLDER/$folder
        ../../../../../../bin/high_precision_pso c ../../$tmpconffile ;
        cd ../..
        rm $tmpconffile
    done
done
