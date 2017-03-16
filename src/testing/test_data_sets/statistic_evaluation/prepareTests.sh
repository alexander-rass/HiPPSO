#!/bin/bash

../../../high_precision_pso c statistic_configfile.conf ;
if [ -d reference_data ] ; then
	rm -r reference_data
fi
mkdir reference_data
mv *.backup reference_data
mv *STAT* reference_data
mv *confBU reference_data
rm statisticTest.log
