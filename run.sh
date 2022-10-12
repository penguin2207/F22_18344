#!/bin/bash

#ewirthap run.sh

BENCHMARK=$1
COMMAND=${@:2}
PINTOOL="/afs/andrew.cmu.edu/usr2/ewirthap/private/18344/lab2/obj-intel64/memory-hierarchy.so"

L1_SIZE="32768"
L1_LINE_SIZE="8"
L1_ASSOC="2"

cd "/afs/andrew.cmu.edu/usr2/ewirthap/private/18344/lab2"
mkdir -p results/test1
cd -
RESULT_FILE="/afs/andrew.cmu.edu/usr2/ewirthap/private/18344/lab2/results/test1/${BENCHMARK}.csv"
pin -t ${PINTOOL} -o ${RESULT_FILE} -l1c ${L1_SIZE} -l1b ${L1_LINE_SIZE} -l1a ${L1_ASSOC} -- ${COMMAND}