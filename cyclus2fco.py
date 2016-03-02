#! /usr/bin/env python
from __future__ import print_function, unicode_literals
import sys
import subprocess
import io
import numpy as np
import re
global r_power_E
global r_deployed
global r_flow
global c_inv
global wr_inv
t_4 = [ [], [], [], []]
t_4_4 = [ t_4, t_4, t_4, t_4]
r_deployed = [ [], [], [], []]
r_power_E = [ [], [], [], []]
r_flow = [ [], [], [], [] ]
c_inv = [ [ [], [], [], [] ], [ [], [], [], [] ], [ [], [], [], [] ], [ [], [], [], [] ]]
wr_inv = [ [ [], [], [], [] ], [ [], [], [], [] ], [ [], [], [], [] ], [ [], [], [], [] ]]


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
  return matrix[1:]

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
    elif hd_id == "CO" : read_cooling(line_hd, line_def)
    elif hd_id == "WR" : read_waiting_repro(line_hd, line_def)
    elif hd_id == "ST" : print("storage")
    elif hd_id == "WT" : print("waste")
    else : print("bad keyword in: ", line )
#def write_outputfile():


def read_reactor(hd, info):
  f_name  = []        #fuel
  for i in range(4):  #max 4 differents fuel
    f_name.append([])
  r_name = []         #reactor proto name
  r_lt = ""           #reactor lifetime
  r_id = int(hd[2])   #reactor number
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
  r_power_E[r_id] = cyan(cmd)
  #get build intel
  for name in r_name:
    cmd = "cyan -db cyclus.sqlite built "
    cmd += name
    r_deployed[r_id] += cyan(cmd)
  #get fresh fuel intel
  for name in r_name:
    cmd = "cyan -db cyclus.sqlite flow -to "
    cmd += name
    r_flow[r_id] += cyan(cmd)

  for truc in r_flow[r_id]:
    print(truc)


def read_waiting_repro(hd, info):
  wr_name = info.split(',')
  wr_id, wr_r_id, wr_f_id = hd.split('_')
  # read Waiting Repro information

  r_id = int(wr_r_id[1])
  f_id = int(wr_f_id[1])

  for name in wr_name:
    cmd = "cyan -db cyclus.sqlite inv  "
    cmd += name
    wr_inv[r_id][f_id] += cyan(cmd)

#  for truc in wr_inv[r_id][f_id]:
#   print(truc)



def read_cooling(hd, info):
  c_name = info.split(',')
  c_id, c_r_id, c_f_id = hd.split('_')
  # read Cooling information

  r_id = int(c_r_id[1])
  f_id = int(c_f_id[1])

  for name in c_name:
    cmd = "cyan -db cyclus.sqlite inv  "
    cmd += name
    c_inv[r_id][f_id] += cyan(cmd)





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