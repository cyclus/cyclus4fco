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
    if   hd_id == "RE" : read_reactor(line_hd, line_def)
    elif hd_id == "RP" : print("repro")
    elif hd_id == "CO" : print("cooling")
    elif hd_id == "WR" : print("waiting repro")
    elif hd_id == "ST" : print("storage")
    elif hd_id == "WT" : print("waste")
    else : print("bad keyword in: ", line )
#def write_outputfile():


def read_reactor(hd, info):

  fuelname = []

  f_name  = []
  for i in range(4):
    f_name.append([])

  r_name = []
  r_lt = ""
  r_id = hd[2]
  r_info = info.split()


# read Reactor information
  for kw in r_info:
    kw_id = kw[0:2]

# get LifeTime
    if kw_id == "LT":
      r_lt = kw[3:]
      print(r_lt)

# get reactor Name
    if kw_id == "NA":
      r_name = kw[3:].split(',')
      for name in r_name:
        print(name)
      print("\n")

# get Fuel Name
    if re.match("(F[0-9])",kw_id) :
      f_id = int(kw_id[1])-1

      name_tmp = kw[3:].split(',')
      for name in name_tmp:
        f_name[f_id].append(name)
      for name in f_name[f_id]:
        print(name)
      print("\n")

#get Power intel
  cmd = "cyan -db cyclus.sqlite power "
  for name in r_name:
    cmd += "-proto="
    cmd += name
    cmd += " "
  r_power_E = cyan(cmd)

#get build intel
cmd = "cyan -db cyclus.sqlite power "
  for name in r_name:
    cmd += "-proto="
    cmd += name
    cmd += " "
r_power_E = cyan(cmd)



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