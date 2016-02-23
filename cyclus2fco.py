#! /usr/bin/env python
from __future__ import print_function, unicode_literals
import sys
import subprocess
import io
import re


def read_input(input):
  print(input)


def read( parameters ):
  print(paramters)


#def write_outputfile():


def main():
  output = "echo "
  prog = 'cyan'
  db = 'cyclus.sqlite'
  cmd = prog + ' -db ' + db + ' trans'
  print(cmd)

  output = subprocess.check_output(cmd.split())
  buf = output.decode("utf-8")
  #  line = buf.readline()
  # print(line)
  print(buf.__class__)
  lines = buf.splitlines()
  matrix = []

  print(len(lines))
  for line in lines:
    cols = line.split()
    matrix.append(cols)

  read_input(sys.argv[1])
#  print(len(matrix))



#cols = line.split()
#print(len(cols))

# print(cols.__class__)

#matrix = []

#matrix.append(cols)

# print(matrix)
main()