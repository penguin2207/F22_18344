#!/bin/python

#ewirthap run.py

import os
import sys

benchmark = sys.argv[1]
command = ' '.join(sys.argv[2:])

pintool = '/afs/andrew.cmu.edu/usr2/ewirthap/private/18344/lab2/obj-intel64/memory-hierarchy.so'

L1_SIZE = 1024
L1_SETS = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024]
L1_ASSOC = [512, 256, 128, 64, 32, 16, 8, 4, 2, 1]
L1_POL = [0, 1, 2]
L2_SIZE = 2048
COUNT=0
VAL=2

for P in L1_POL:
    for A in L1_ASSOC:
        for S in L1_SETS:
            if COUNT < VAL:
                result_file = os.path.join('/afs/andrew.cmu.edu/usr2/ewirthap/private/18344/lab2/results/1024_L1_L2/', benchmark + '.csv')
                lab_options = '-l1c {0} -l1a {1} -l1s {2} -l1p {3} -l2c {4} -l2a {1} -l2s {2} -l2p {3}'.format(L1_SIZE, A, S, P, L2_SIZE)
                options = '-o %s %s' % (result_file, lab_options)
                pin_cmd = 'pin -t %s %s -- %s' % (pintool, options, command)
                os.system(pin_cmd)
            COUNT+=1
        COUNT=0
        VAL+=1
    VAL=2



L1_SIZE = 4096
L1_SETS = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096]
L1_ASSOC = [512, 256, 128, 64, 32, 16, 8, 4, 2, 1]
L1_POL = [0, 1, 2]
L2_SIZE = 8192
COUNT=0
VAL=4

for P in L1_POL:
    for A in L1_ASSOC:
        for S in L1_SETS:
            if COUNT < VAL:
                result_file = os.path.join('/afs/andrew.cmu.edu/usr2/ewirthap/private/18344/lab2/results/4096_L1_L2/', benchmark + '.csv')
                lab_options = '-l1c {0} -l1a {1} -l1s {2} -l1p {3} -l2c {4} -l2a {1} -l2s {2} -l2p {3}'.format(L1_SIZE, A, S, P, L2_SIZE)
                options = '-o %s %s' % (result_file, lab_options)
                pin_cmd = 'pin -t %s %s -- %s' % (pintool, options, command)
                os.system(pin_cmd)
            COUNT+=1
        COUNT=0
        VAL+=1
    VAL=4


L1_SIZE = 16384
L1_SETS = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384]
L1_ASSOC = [512, 256, 128, 64, 32, 16, 8, 4, 2, 1]
L1_POL = [0, 1, 2]
L2_SIZE = 32768
COUNT=0
VAL=6

for P in L1_POL:
    for A in L1_ASSOC:
        for S in L1_SETS:
            if COUNT < VAL:
                result_file = os.path.join('/afs/andrew.cmu.edu/usr2/ewirthap/private/18344/lab2/results/16384_L1_L2/', benchmark + '.csv')
                lab_options = '-l1c {0} -l1a {1} -l1s {2} -l1p {3} -l2c {4} -l2a {1} -l2s {2} -l2p {3}'.format(L1_SIZE, A, S, P, L2_SIZE)
                options = '-o %s %s' % (result_file, lab_options)
                pin_cmd = 'pin -t %s %s -- %s' % (pintool, options, command)
                os.system(pin_cmd)
            COUNT+=1
        COUNT=0
        VAL+=1
    VAL=6

L1_SIZE = 65536
L1_SETS = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536]
L1_ASSOC = [512, 256, 128, 64, 32, 16, 8, 4, 2, 1]
L1_POL = [0, 1, 2]
L2_SIZE = 131072
COUNT=0
VAL=8

for P in L1_POL:
    for A in L1_ASSOC:
        for S in L1_SETS:
            if COUNT < VAL:
                result_file = os.path.join('/afs/andrew.cmu.edu/usr2/ewirthap/private/18344/lab2/results/65536_L1_L2/', benchmark + '.csv')
                lab_options = '-l1c {0} -l1a {1} -l1s {2} -l1p {3} -l2c {4} -l2a {1} -l2s {2} -l2p {3}'.format(L1_SIZE, A, S, P, L2_SIZE)
                options = '-o %s %s' % (result_file, lab_options)
                pin_cmd = 'pin -t %s %s -- %s' % (pintool, options, command)
                os.system(pin_cmd)
            COUNT+=1
        COUNT=0
        VAL+=1
    VAL=8


L1_SIZE = 262144
L1_SETS = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144]
L1_ASSOC = [512, 256, 128, 64, 32, 16, 8, 4, 2, 1]
L1_POL = [0, 1, 2]
L2_SIZE = 524288
COUNT=0
VAL=10

for P in L1_POL:
    for A in L1_ASSOC:
        for S in L1_SETS:
            if COUNT < VAL:
                result_file = os.path.join('/afs/andrew.cmu.edu/usr2/ewirthap/private/18344/lab2/results/262144_L1_L2/', benchmark + '.csv')
                lab_options = '-l1c {0} -l1a {1} -l1s {2} -l1p {3} -l2c {4} -l2a {1} -l2s {2} -l2p {3}'.format(L1_SIZE, A, S, P, L2_SIZE)
                options = '-o %s %s' % (result_file, lab_options)
                pin_cmd = 'pin -t %s %s -- %s' % (pintool, options, command)
                os.system(pin_cmd)
            COUNT+=1
        COUNT=0
        VAL+=1
    VAL=10

    
L1_SIZE = 1048576
L1_SETS = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576]
L1_ASSOC = [512, 256, 128, 64, 32, 16, 8, 4, 2, 1]
L1_POL = [0, 1, 2]
L2_SIZE = 2097152
COUNT=0
VAL=12

for P in L1_POL:
    for A in L1_ASSOC:
        for S in L1_SETS:
            if COUNT < VAL:
                result_file = os.path.join('/afs/andrew.cmu.edu/usr2/ewirthap/private/18344/lab2/results/1048576_L1_L2/', benchmark + '.csv')
                lab_options = '-l1c {0} -l1a {1} -l1s {2} -l1p {3} -l2c {4} -l2a {1} -l2s {2} -l2p {3}'.format(L1_SIZE, A, S, P, L2_SIZE)
                options = '-o %s %s' % (result_file, lab_options)
                pin_cmd = 'pin -t %s %s -- %s' % (pintool, options, command)
                os.system(pin_cmd)
            COUNT+=1
        COUNT=0
        VAL+=1
    VAL=12

L1_SIZE = 4194304
L1_SETS = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304]
L1_ASSOC = [512, 256, 128, 64, 32, 16, 8, 4, 2, 1]
L1_POL = [0, 1, 2]
L2_SIZE = 524288
COUNT=0
VAL=14

for P in L1_POL:
    for A in L1_ASSOC:
        for S in L1_SETS:
            if COUNT < VAL:
                result_file = os.path.join('/afs/andrew.cmu.edu/usr2/ewirthap/private/18344/lab2/results/4194304_L1_L2/', benchmark + '.csv')
                lab_options = '-l1c {0} -l1a {1} -l1s {2} -l1p {3} -l2c {4} -l2a {1} -l2s {2} -l2p {3}'.format(L1_SIZE, A, S, P, L2_SIZE)
                options = '-o %s %s' % (result_file, lab_options)
                pin_cmd = 'pin -t %s %s -- %s' % (pintool, options, command)
                os.system(pin_cmd)
            COUNT+=1
        COUNT=0
        VAL+=1
    VAL=14