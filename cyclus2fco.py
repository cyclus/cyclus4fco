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




#Half-life > 1h
nucs_U = "230U,231U,232U,233U,234U,235U,236U,237U,238U,240U"
nucs_Pu_list = [ "238pu", "239pu", "240pu", "241pu", "242pu"]

nucs_Pu = "234Pu,235Pu,236Pu,237Pu,238Pu,239Pu,240Pu,241Pu,242Pu,243Pu,244Pu,245Pu,246Pu,247Pu"
nucs_Am = "239Am,240Am,241Am,242Am,243Am,244Am,245Am"
nucs_Cm = "238Cm,240Cm,241Cm,242Cm,243Am,244Cm,245Cm,246Cm,247Cm,248Cm,250Cm"
nucs_Np = "234Np,235Np,236Np,237Np,238Np,239Np"
nucs_FP = "137Cs"
nucs_MA = nucs_Am + "," + nucs_Cm + "," + nucs_Np
nucs_name = [ nucs_U, nucs_Pu, nucs_MA, nucs_FP]




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

def fill_the_matrix(matrix_to_fill, matrix_row):
  print( "TOTO")


def translate_info(input, size, lengh):
  output = np.zeros(lengh)
  for couple in input:
    output[int(couple[0])] += float(couple[1])
  return output


def build_cmd(inputline):
  print(inputline)


def recover_info(line):
  if len(line.split(' ',1)) != 2:
          print("badline", line)
  else :
    line_hd,line_def = line.split(' ',1)
    hd_id = line_hd[0:2]
    if   hd_id == "RE" : read_reactor(line_hd, line_def)
    elif hd_id == "RP" : read_reprocessing(line_hd, line_def)
    elif hd_id == "CO" : read_cooling(line_hd, line_def)
    elif hd_id == "WR" : read_waiting_repro(line_hd, line_def)
    elif hd_id == "ST" : read_storage(line_hd, line_def)
    elif hd_id == "WT" : read_waste(line_hd, line_def)
    else : print("bad keyword in: ", line )
#def write_outputfile():


def read_reactor(hd, info):
  r_deployed = [ [], [], [], []]
  r_power_E = [ [], [], [], []]
  r_flow = [ [], [], [], [] ]
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
  # get reactor Name
    if kw_id == "NA":
      r_name = kw[3:].split(',')
  # get Fuel Name
    if re.match("(F[0-9])",kw_id) :
      f_id = int(kw_id[1])-1
      name_tmp = kw[3:].split(',')
      for name in name_tmp:
        f_name[f_id].append(name)
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

  translate_info(r_flow[r_id], 2,timestep)



def read_reprocessing(hd, info):
  rp_inv = [[ [], [], [], [] ],
            [ [], [], [], [] ],
            [ [], [], [], [] ],
            [ [], [], [], [] ]]
  rp_name = []
  rp_commods = []
  rp_id, rp_r_id, rp_f_id = hd.split('_')
  # read repro id information
  r_id = int(rp_r_id[1])
  f_id = int(rp_f_id[1])
  # read repro information
  rp_info = info.split()
  for kw in rp_info:
    kw_id = kw[0]
    # get LifeTime
    if kw_id == "N":
      rp_name = kw[1:].split(',')
    # get reactor Name
    if kw_id == "C":
      rp_commods = kw[1:].split(',')

    for name in rp_name:
      for commods in rp_commods:
        rp_inv[r_id][f_id] += cyan("cyan -db cyclus.sqlite flow -to " + name + " -commod=" + commods  )



def read_waiting_repro(hd, info):
  wr_inv = [[ [], [], [], [] ],
            [ [], [], [], [] ],
            [ [], [], [], [] ],
            [ [], [], [], [] ]]
  wr_name = info.split(',')
  wr_id, wr_r_id, wr_f_id = hd.split('_')
  # read Waiting Repro information
  r_id = int(wr_r_id[1])
  f_id = int(wr_f_id[1])
  for name in wr_name:
    cmd = "cyan -db cyclus.sqlite inv "
    cmd += name
    wr_inv[r_id][f_id] += cyan(cmd)

def read_cooling(hd, info):
  c_inv = [[ [], [], [], [] ],
           [ [], [], [], [] ],
           [ [], [], [], [] ],
           [ [], [], [], [] ]]
  c_name = info.split(',')
  c_id, c_r_id, c_f_id = hd.split('_')
  # read Cooling information
  r_id = int(c_r_id[1])
  f_id = int(c_f_id[1])
  for name in c_name:
    cmd = "cyan -db cyclus.sqlite inv "
    cmd += name
    c_inv[r_id][f_id] += cyan(cmd)

def read_storage(hd, info):
  st_inv = [ [], [], [], [] ]
  st_pu_inv = [ [], [], [], [] ]
  st_name = info.split(',')
  # read Cooling information
  for name in st_name:
    cmd = "cyan -db cyclus.sqlite inv "
    for i in range(4):
      st_inv[i] += cyan(cmd + "-nucs=" + nucs_name[i] +" " + name)
  for name in st_name:
    cmd = "cyan -db cyclus.sqlite inv "
    for i in range(4):
      st_pu_inv[i] += cyan(cmd + "-nucs=" + nucs_Pu_list[i] +" " + name)


def read_waste(hd, info):
  wt_inv = [ [], [], [], [] ]
  wt_name = info.split(',')
  # read Cooling information
  for name in wt_name:
    cmd = "cyan -db cyclus.sqlite inv "
    for i in range(4):
      wt_inv[i] += cyan(cmd + "-nucs=" + nucs_name[i] +" " + name)


def build_infotable():
  print( "tt")


def get_timestep():
  cmd = "cyan -db cyclus.sqlite infile |grep duration | cut -d\< -f2 |cut -d\> -f2"
  ps = subprocess.Popen(cmd,shell=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
  timestep = int(ps.communicate()[0][:-1])
  return timestep

def main():
  global timestep
  if len(sys.argv) != 2 :
    print("missing argument !!")
    quit()

  timestep = get_timestep()
  input_list = read_input(sys.argv[1])
  for line in input_list:
    if line != '\n':
       recover_info(line)

  build_infotable()






main()