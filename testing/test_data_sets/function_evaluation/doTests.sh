#!/bin/bash

reference="reference_data"
if [ -d $reference ] ; then
	echo "Start test.";
else
	echo "Missing reference data.";
	exit 1;
fi

exitCode=0
fileCount=$(ls | wc -l)
for file in $reference/*.conf ; do
	../../../high_precision_pso c $file
	receivedExitCode=$? ;
	expectedExitCode=0;
	if [ $receivedExitCode -ne $expectedExitCode ] ; then
		echo "FAILURE DETECTED"
		echo "Exit code of the program was not as expected for $file."
		echo "Expected: $expectedExitCode"
		echo "Received: $receivedExitCode"
		exitCode=1
	fi
done
rm *.log
for file in $reference/* ; do
	if [ ${file: -5} != ".conf" ] ; then
		checkFile=$(echo $file | sed "s#$reference/##");
		if [ -f $checkFile ] ; then
			differLines=$(diff $file $checkFile | wc -l)
			if [ $differLines -eq 0 ] ; then
				rm $checkFile
				else
				echo "FAILURE DETECTED"
				echo "$checkFile does not match!"
				exitCode=1
			fi
		else
			echo "FAILURE DETECTED"
			echo "Missing file $checkFile."
			exitCode=1
		fi
	fi
done
newfileCount=$(ls | wc -l)

if [ $newfileCount -ne $fileCount ] ; then
	echo "FAILURE DETECTED"
	echo "Additional files generated!"
	exitCode=1
fi

if [ $exitCode -eq 0 ] ; then
	echo "Test succeeded."
else
	echo "TEST FAILED!!"
fi

exit $exitCode
