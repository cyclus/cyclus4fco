#! /usr/bin/env python
from __future__ import print_function, unicode_literals
import sys
import subprocess
import io
import re


def read_input(input):
  f = open(input, 'r')
  matrix = []
  for line in f:
    print(line)
    matrix.append(line)

  return matrix

def read( parameters ):
  print(paramters)


def recover_info( cmd ):
  output = subprocess.check_output(cmd.split())
  buf = output.decode("utf-8")
  lines = buf.splitlines()
  matrix = []
  for line in lines:
    cols = line.split()
    matrix.append(cols)
  return matrix

def treat_the_matrix( matrix)



def build_cmd(inputline)


#def write_outputfile():


def main():

  if len(sys.argv) != 2 :
    print("missing argument !!")
  #   quit()



  prog = 'cyan'
  db = 'cyclus.sqlite'
  cmd = prog + ' -db ' + db + ' trans'
  matrix = recover_info(cmd)

  read_input(sys.argv[1])
#  print(len(matrix))



#cols = line.split()
#print(len(cols))

# print(cols.__class__)

#matrix = []

#matrix.append(cols)

# print(matrix)
main()