#!/bin/bash
for file in *.gnuplot.txt ; do
	if [ -f "$file" ]; then
		gnuplot $file
	fi
done
