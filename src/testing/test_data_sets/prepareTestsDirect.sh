#!/bin/bash

NUMTESTS=$1

if [ -d reference_data ] ; then
	rm -r reference_data
fi
mkdir reference_data

for TESTID in $(seq 1 $NUMTESTS) ; do
    mkdir reference_data/run$TESTID
    cd reference_data/run$TESTID

    ../../../../../../bin/high_precision_pso c ../../configfiles/configfile$TESTID.conf ;

    cd ../..
done
