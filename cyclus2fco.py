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
    #    print(line)
    matrix.append(line)

  return matrix

def read( parameters ):
  print(paramters)


def cyan( cmd ):
  output = subprocess.check_output(cmd.split())
  buf = output.decode("utf-8")
  lines = buf.splitlines()
  matrix = []
  for line in lines:
    cols = line.split()
    matrix.append(cols)
  return matrix

def treat_the_matrix(matrix):
  print(matrix)


def build_cmd(inputline):
  print(inputline)


def recover_info(line):
  if len(line.split(' ',1)) != 2:
          print("badline", line)
  else :
    line_hd,line_def = line.split(' ',1)
    hd_id = line_hd[0:2]
    if hd_id == 'RE' : print("reactor")
    elif hd_id == 'RP' : print("repro")
    elif hd_id == 'CO' : print("cooling")
    elif hd_id == 'WR' : print("waiting repro")
    elif hd_id == 'ST' : print("storage")
    elif hd_id == 'WT' : print("waste")
    else : print("bad keyword in: ", line )
#def write_outputfile():


def main():

  if len(sys.argv) != 2 :
    print("missing argument !!")
    quit()

  input_list = read_input(sys.argv[1])


  for line in input_list:
    if line != '\n':
      recover_info(line)

#  prog = 'cyan'
#  db = 'cyclus.sqlite'
#  cmd = prog + ' -db ' + db + ' trans'
# matrix = recover_info(cmd)

#  read_input(sys.argv[1])
#  print(len(matrix))



#cols = line.split()
#print(len(cols))

# print(cols.__class__)

#matrix = []

#matrix.append(cols)

# print(matrix)
main()