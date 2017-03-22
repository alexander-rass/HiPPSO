#!/bin/bash

numfunctions=$(cat function_options.conf | wc -l);
for line in $(seq 1 $numfunctions) ;
do
	file=run$line.conf ;
	cat base_configfile.conf > $file;
	functionDescription=$(head -n$line function_options.conf | tail -n1);
	echo "function $functionDescription" >> $file;
	echo "showNamedStatistic FuncEvalPos reduce dimension functionEvaluation $functionDescription position" >> $file;
	echo "showNamedStatistic FuncEvalLocAt reduce dimension functionEvaluation $functionDescription localAttractor" >> $file;

	echo "fileprefix run$line" >>  $file;
	../../../high_precision_pso c $file ;
done
if [ -d reference_data ] ; then
	rm -r reference_data
fi
mkdir reference_data
mv *.backup reference_data
mv *STAT* reference_data
mv *confBU reference_data
rm run*.log
mv run*.conf reference_data
