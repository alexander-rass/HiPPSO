#!/bin/bash

ITERATIONS="142"

for TESTID in 1 2 ; do
    echo Testseries $TESTID
    REFFOLDER="reference_data$TESTID"
    FUNCTIONOPTIONS="function_options$TESTID.conf"
    # create clean folder for reference data
    if [ -d $REFFOLDER ] ; then
        rm -r $REFFOLDER
    fi
    mkdir $REFFOLDER

    numfunctions=$(cat $FUNCTIONOPTIONS | wc -l);
    for line in $(seq 1 $numfunctions) ;
    do
        tmpconffile=tmp.conf ;
        cat base_configfile$TESTID.conf > $tmpconffile;
        functionDescription=$(head -n$line $FUNCTIONOPTIONS | tail -n1);
        echo "function $functionDescription" >> $tmpconffile;
        echo "showNamedStatistic FuncEvalPos reduce dimension functionEvaluation $functionDescription position" >> $tmpconffile;
        echo "showNamedStatistic FuncEvalLocAt reduce dimension functionEvaluation $functionDescription localAttractor" >> $tmpconffile;
        echo "steps $ITERATIONS" >> $tmpconffile
        folder=run$line ;
        mkdir $REFFOLDER/$folder
        cd $REFFOLDER/$folder
        ../../../../../../bin/high_precision_pso c ../../$tmpconffile ;
        cd ../..
        rm $tmpconffile
    done
done
