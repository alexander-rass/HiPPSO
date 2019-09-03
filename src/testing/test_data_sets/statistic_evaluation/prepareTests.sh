#!/bin/bash

if [ -d reference_data ] ; then
	rm -r reference_data
fi
mkdir reference_data

for TESTID in 1 2 ; do
    mkdir reference_data/run$TESTID
    cd reference_data/run$TESTID

    ../../../../../../bin/high_precision_pso c ../../statistic_configfile$TESTID.conf ;

    cd ../..
done
