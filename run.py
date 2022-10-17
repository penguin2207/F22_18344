#!/bin/python

#ewirthap run.py

import os
import sys

benchmark = sys.argv[1]
command = ' '.join(sys.argv[2:])

pintool = '/afs/andrew.cmu.edu/usr2/ewirthap/private/18344/lab2/obj-intel64/memory-hierarchy.so'

L1_SIZE = 4096
L1_SETS = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512]
L1_ASSOC = [8, 4, 2, 1]
L1_POL = [0, 1, 2]
COUNT=0

for P in L1_POL:
    for A in L1_ASSOC:
        for S in L1_SETS:
                result_file = os.path.join('/afs/andrew.cmu.edu/usr2/ewirthap/private/18344/lab2/results/4096_L1_Only_Real/', benchmark + '.csv')
                lab_options = '-l1c {0} -l1a {1} -l1s {2} -l1p {3}'.format(L1_SIZE, A, S, P)
                options = '-o %s %s' % (result_file, lab_options)
                pin_cmd = 'pin -t %s %s -- %s' % (pintool, options, command)
                os.system(pin_cmd)


L1_SIZE = 16384
L1_SETS = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048]
L1_ASSOC = [16, 8, 4, 2, 1]
L1_POL = [0, 1, 2]
COUNT=0
VAL=11

for P in L1_POL:
    for A in L1_ASSOC:
        for S in L1_SETS:
            if COUNT < VAL:
                result_file = os.path.join('/afs/andrew.cmu.edu/usr2/ewirthap/private/18344/lab2/results/16384_L1_Only_Real/', benchmark + '.csv')
                lab_options = '-l1c {0} -l1a {1} -l1s {2} -l1p {3}'.format(L1_SIZE, A, S, P)
                options = '-o %s %s' % (result_file, lab_options)
                pin_cmd = 'pin -t %s %s -- %s' % (pintool, options, command)
                os.system(pin_cmd)
            COUNT+=1
        COUNT=0
        VAL+=1
    VAL=11


L1_SIZE = 262144
L1_SETS = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768]
L1_ASSOC = [32, 16, 8, 4, 2, 1]
L1_POL = [0, 1, 2]
COUNT=0
VAL=14

for P in L1_POL:
    for A in L1_ASSOC:
        for S in L1_SETS:
            if COUNT < VAL:
                result_file = os.path.join('/afs/andrew.cmu.edu/usr2/ewirthap/private/18344/lab2/results/262144_L1_Only_Real/', benchmark + '.csv')
                lab_options = '-l1c {0} -l1a {1} -l1s {2} -l1p {3}'.format(L1_SIZE, A, S, P)
                options = '-o %s %s' % (result_file, lab_options)
                pin_cmd = 'pin -t %s %s -- %s' % (pintool, options, command)
                os.system(pin_cmd)
            COUNT+=1
        COUNT=0
        VAL+=1
    VAL=14


L1_SIZE = 4194304
L1_SETS = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144]
L1_ASSOC = [16, 8, 4, 2, 1]
L1_POL = [0, 1, 2]

for P in L1_POL:
    for A in L1_ASSOC:
        for S in L1_SETS:
                result_file = os.path.join('/afs/andrew.cmu.edu/usr2/ewirthap/private/18344/lab2/results/4194304_L1_Only_Real/', benchmark + '.csv')
                lab_options = '-l1c {0} -l1a {1} -l1s {2} -l1p {3}'.format(L1_SIZE, A, S, P)
                options = '-o %s %s' % (result_file, lab_options)
                pin_cmd = 'pin -t %s %s -- %s' % (pintool, options, command)
                os.system(pin_cmd)