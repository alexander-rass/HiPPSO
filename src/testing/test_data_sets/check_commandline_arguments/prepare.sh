#!/bin/bash

./createDataRestartAll.sh reference_data
mkdir reference_data/usage
../../../../bin/high_precision_pso > reference_data/usage/usage.txt
exit 0
