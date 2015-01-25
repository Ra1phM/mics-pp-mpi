#!/usr/bin/python

import os
import sys

print '*****************'
print 'HPC Launch Script'
print 'Usage: python launch_hpc_benchmark.py <executable> <numberOfProcesses>'
print '*****************'

path_c_file_to_run = str(sys.argv[1])

start = 1;
number_of_nodes = int(sys.argv[2])

setup_command_sequence = [
    'rm -f stats.dat'
]

print '--- Preparing Runs ---'
for cmd in setup_command_sequence:
    print 'Running: '+ cmd
    os.system(cmd)

print '--- Running '+str(number_of_nodes)+' times '+path_c_file_to_run+' ---'

for i in range(start, number_of_nodes + 1):
    #command_1 = 'export OMP_NUM_THREADS='+str(i)
    os.environ["OMP_NUM_THREADS"] = str(i)
    command_2 = './' + path_c_file_to_run
    print 'Running 1/2: '+ command_1
    os.system(command_1)
    print 'Running 2/2: '+ command_2
    os.system(command_2)

print '--- End Running ---'