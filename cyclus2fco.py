#! /usr/bin/env python
from __future__ import print_function, unicode_literals
import xlwings as xw
import os
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


fco_sheet = ['Data-Front End', 'Data-Reactor', 'Data-Recycle', 'Data-Inventories', 'Data-Economics', 'Data-Wildcard', 'Graph_Data', 'Settings', 'Pres_Graphs', 'Graphs (C1-4)', 'Version Notes']

#Half-life > 1h
nucs_Pu_list = [ "238pu", "239pu", "240pu", "241pu", "242pu"]

nucs_U = "230U,231U,232U,233U,234U,235U,236U,237U,238U,240U"
nucs_Pu = "234Pu,235Pu,236Pu,237Pu,238Pu,239Pu,240Pu,241Pu,242Pu,243Pu,244Pu,245Pu,246Pu,247Pu"
nucs_Am = "239Am,240Am,241Am,242Am,243Am,244Am,245Am"
nucs_Cm = "238Cm,240Cm,241Cm,242Cm,243Am,244Cm,245Cm,246Cm,247Cm,248Cm,250Cm"
nucs_Np = "234Np,235Np,236Np,237Np,238Np,239Np"
nucs_FP = "H3,GE74,AS75,GE76,SE77,SE78,SE79,SE80,BR81,SE82,KR82,KR83,KR84,KR85,RB85,KR86,SR86,RB87,SR88,SR89,Y89,SR90,ZR90,SR91,Y91,ZR91,ZR92,Y93,ZR93,ZR94,ZR95,NB95,MO95,ZR96,MO96,ZR97,MO97,MO98,MO99,TC99,TC99M,MO100,RU100,RU101,RU102,RU103,RH103,RU104,PD104,RU105,RH105,PD105,RU106,PD106,PD107,PD108,PD109,AG109,PD110,AG110,CD110,AG111,CD111,CD112,CD113,CD113,CD114,IN115,SN115,CD116,SN116,SN117,SN118,SN119,SN120,SB121,SN122,TE122,SN123,SB123,SN124,SB124,TE124,SN125,SB125,TE125,TE125,SN126,TE126,SB127,TE127,I127,TE128,XE128,TE129,I129,TE130,XE130,TE131,I131,XE131,XE131,TE132,I132,XE132,I133,XE133,XE133,CS133,XE134,CS134,BA134,I135,XE135,CS135,BA135,XE136,CS136,BA136,CS137,BA137,BA138,LA138,LA139,BA140,LA140,CE140,LA141,CE141,PR141,CE142,ND142,CE143,PR143,ND143,CE144,ND144,PR145,ND145,ND146,ND147,PM147,SM147,ND148,PM148,SM148,PM149,SM149,ND150,SM150,PM151,SM151,SM152,SM153,EU153,SM154,EU154,GD154,EU155,GD155,EU156,GD156,GD157,GD158,TB159,GD160,TB160,DY160,DY161,DY162,DY163,DY164,HO165,ER166"
nucs_MA = nucs_Am + "," + nucs_Cm + "," + nucs_Np



def push_in_fco_excel(matrix, sheet, column, start_row):
  for i in range (len(matrix)):
    cmd = "sed 's/\(r=\""+column + str(start_row+i)
    cmd += "\" .......<v>\)[^<]*/\\1"
    cmd += str(matrix[i]) + "/' _tmp/xl/worksheets/" + sheet + " > _tmp/xl/worksheets/sheet_tmp.xml"
    os.system(cmd)
    os.system("mv _tmp/xl/worksheets/sheet_tmp.xml _tmp/xl/worksheets/" + sheet)



def open_xslm(filename):
  cmd = 'unzip '+ filename + ' -d _tmp/'
  output = subprocess.check_output(cmd.split())



def save_xslm(filename):
  cmd = 'cd _tmp; zip -r ../'+ filename + ' *'
  os.system(cmd + ' >/dev/null')
  cmd ='rm -rf _tmp'
  os.system(cmd)



def read_input(input):
  f = open(input, 'r')
  matrix = []
  for line in f:
    matrix.append(line)
  return matrix



def cyan( cmd ):
  output = subprocess.check_output(cmd.split())
  buf = output.decode("utf-8")
  lines = buf.splitlines()
  matrix = []
  for line in lines:
    cols = line.split()
    matrix.append(cols)
  return matrix[1:]



def translate_info(input, size, lengh):
  output = np.zeros(lengh, dtype=np.float64)
  for couple in input:
    output[int(couple[0])] += float(couple[1])
  return output



def month2year(input, cumulativ, rate):
  lengh = len(input)
  output = np.zeros(int(lengh/12), dtype=np.float64)
  for i in range(lengh):
    if i % 12 == 0:
      output[int(i/12)] = input[i]
    elif cumulativ == 1:
      output[int(i/12)] += input[i]

  if(rate == 1):
    output = output/12.
  return output



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


def read_reactor(hd, info):

  r_sheet = 'sheet2.xml'
  FCO_reactor_E_position = [ 'C', 'H', 'M', 'R']
  FCO_reactor_I_position = [ 'AD', 'AI', 'AN', 'AS']
  FCO_reactor_B_position = [ 'AY', 'BD', 'BI', 'BN']
  FCO_reactor_R_position = [ 'BT', 'BY', 'CD', 'CI']
  r_sheet_flows = 'sheet1.xml'
  FCO_reactor_LR_position = [[ 'CQ', 'CV', 'DA', 'DF'],
                             [ 'DK', 'DP', 'DU', 'DZ'],
                             [ 'EE', 'EJ', 'EO', 'ET'],
                             [ 'EY', 'FD', 'FI', 'FN']]

  r_built = []
  r_deployed = []
  r_retired = np.zeros(timestep, dtype=np.float64)
  r_power_E = []
  f_name  = []        #fuel
  for i in range(4):  #max 4 differents fuel
    f_name.append([])

  r_name = []         #reactor proto name
  r_lt = ""           #reactor lifetime
  r_cap =""           #reactor individual capacity factor
  r_p = ""             #reactor individual power factor
  r_id = int(hd[2]) -1   #reactor number
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
    # get reactor Capacity
    if kw_id == "CA":
      r_cap = kw[3:]
    # get reactor Capacity
    if kw_id == "PO":
      r_p = kw[3:]

  #get Power intel
  cmd = "cyan -db cyclus.sqlite power "
  for name in r_name:
    cmd += "-proto="
    cmd += name
    cmd += " "
  r_power_E += cyan(cmd)
  r_power = translate_info(r_power_E, 2,timestep)
  r_power_yearly = month2year(r_power, 0, 0)
  r_power_yearly = r_power_yearly/1000
  push_in_fco_excel(r_power_yearly, r_sheet, FCO_reactor_E_position[r_id], 6)

  #get built intel
  for name in r_name:
    cmd = "cyan -db cyclus.sqlite built "
    cmd += name
  r_built += cyan(cmd)
  r_built = translate_info(r_built, 2,timestep)
  r_built_yearly = month2year(r_built, 1, 0)*float(r_p)
  push_in_fco_excel(r_built_yearly, r_sheet, FCO_reactor_B_position[r_id], 6)

  #get deployed intel
  for name in r_name:
    cmd = "cyan -db cyclus.sqlite deployed "
    cmd += name
  r_deployed += cyan(cmd)
  r_deployed = translate_info(r_deployed, 2,timestep)
  r_deployed_yearly = month2year(r_deployed, 0, 0)*float(r_p)
  push_in_fco_excel(r_deployed_yearly, r_sheet, FCO_reactor_I_position[r_id], 6)

  for i in range(timestep)[1:]:
    r_retired[i] = -(r_deployed[i] - r_deployed[i-1] -r_built[i])
  r_retired_yearly = month2year(r_retired, 1, 0)*float(r_p)
  push_in_fco_excel(r_retired_yearly, r_sheet, FCO_reactor_R_position[r_id], 6)

  #get fresh fuel intel
  for name in r_name:
    r_flow = []
    cmd = "cyan -db cyclus.sqlite flow -to "
    cmd += name
    for i in range(4):
      for f_names in f_name[i]:
        cmd2 = cmd
        cmd2 += ' -commod=' + f_names
        r_flow += cyan(cmd2)
        r_flows = translate_info(r_flow, 2,timestep)
        r_flow_yearly = month2year(r_flows, 1, 1)/1000
        push_in_fco_excel(r_flow_yearly, r_sheet_flows, FCO_reactor_LR_position[r_id][i], 6)




def read_reprocessing(hd, info):

  FCO_separation_position = [[ 'FI', 'FN', 'FS', 'FX'],
                             [ 'GC', 'GH', 'GM', 'GR'],
                             [ 'GW', 'HB', 'HG', 'HL'],
                             [ 'HQ', 'HV', 'IA', 'IF']]
  r_sheet_sepatation = 'sheet3.xml'

  rp_inv = []
  rp_name = []
  rp_commods = []
  rp_id, rp_r_id, rp_f_id = hd.split('_')
  # read repro id information
  r_id = int(rp_r_id[1])-1
  f_id = int(rp_f_id[1])-1
  # read repro information
  rp_info = info.split()
  for kw in rp_info:
    kw_id = kw[0:2]
    # get LifeTime
    if kw_id == "NA":
      rp_name = kw[3:].split(',')
   # get reactor Name
    if kw_id == "CO":
      rp_commods = kw[3:].split(',')

  for name in rp_name:
    for commods in rp_commods:
      rp_inv += cyan("cyan -db cyclus.sqlite flow -to " + name + " -commod=" + commods  )
  rp_inv = translate_info(rp_inv, 2,timestep)
  rp_inv_yearly = month2year(rp_inv, 1, 1)/1000
  push_in_fco_excel(rp_inv_yearly, r_sheet_sepatation, FCO_separation_position[r_id][f_id], 6)



def read_waiting_repro(hd, info):

  FCO_waiting_position = [[ 'CF', 'CK', 'CP', 'CU'],
                          [ 'CZ', 'DE', 'DJ', 'DO'],
                          [ 'DT', 'DY', 'ED', 'EI'],
                          [ 'EN', 'ES', 'EX', 'FC']]
  r_sheet_waiting = 'sheet4.xml'

  wr_inv = []
  wr_name = info.split(',')
  wr_id, wr_r_id, wr_f_id = hd.split('_')
  # read Waiting Repro information
  r_id = int(wr_r_id[1])-1
  f_id = int(wr_f_id[1])-1
  for name in wr_name:
    cmd = "cyan -db cyclus.sqlite inv "
    cmd += name
    wr_inv += cyan(cmd)
  wr_inv = translate_info(wr_inv, 2,timestep)
  wr_inv_yearly = month2year(wr_inv, 0, 0)/1000
  push_in_fco_excel(wr_inv_yearly, r_sheet_waiting, FCO_waiting_position[r_id][f_id], 6)



def read_cooling(hd, info):
  FCO_cooling_position = [[ 'C', 'H', 'M', 'R'],
                          [ 'W', 'AB', 'AG', 'AL'],
                          [ 'AQ', 'AV', 'BA', 'BF'],
                          [ 'BK', 'BP', 'BU', 'BZ']]
  r_sheet_cooling = 'sheet4.xml'

  c_inv = []
  c_name = info.split(',')
  c_id, c_r_id, c_f_id = hd.split('_')
  # read Cooling information
  r_id = int(c_r_id[1])-1
  f_id = int(c_f_id[1])-1
  for name in c_name:
    cmd = "cyan -db cyclus.sqlite inv "
    cmd += name
    c_inv += cyan(cmd)
  c_inv = translate_info(c_inv, 2,timestep)
  c_inv_yearly = month2year(c_inv, 0, 0)/1000
  push_in_fco_excel(c_inv_yearly, r_sheet_cooling, FCO_cooling_position[r_id][f_id], 6)


def read_storage(hd, info):
  # filling order is U Pu MA FP
  nucs_name = [ nucs_Pu, nucs_MA ]

  FCO_PUMA_storage_position = ['FI', 'FN']
  FCO_storage_pu_position = ['GD', 'GI', 'GN', 'GS', 'GX']
  r_sheet_storage = 'sheet4.xml'

  if len(hd) == 2 : # read PU and MA storage content
    st_inv = [ [], [] ]
    st_pu_inv = [ [], [] ,[], [], []]
    st_name = info.split(',')
    # read Cooling information
    for name in st_name:
      cmd = "cyan -db cyclus.sqlite inv "
      for i in range(2):
        st_inv[i] += cyan(cmd + "-nucs=" + nucs_name[i] +" " + name)
    for i in range(2):
      st_inv[i] = translate_info(st_inv[i], 2,timestep)
      st_inv_yearly = month2year(st_inv[i], 0, 0)/1000
      push_in_fco_excel(st_inv_yearly, r_sheet_storage, FCO_PUMA_storage_position[i], 6)

    for name in st_name:
      cmd = "cyan -db cyclus.sqlite inv "
      for i in range(5):
        st_pu_inv[i] += cyan(cmd + "-nucs=" + nucs_Pu_list[i] +" " + name)
    for i in range(5):
      st_pu_inv[i] = translate_info(st_pu_inv[i], 2,timestep)
      st_inv_yearly = month2year(st_pu_inv[i], 0, 0)/1000
      push_in_fco_excel(st_inv_yearly, r_sheet_storage, FCO_storage_pu_position[i], 6)
  else : # read RU/DU content
    st_id = hd.split('_')[1]
    if st_id == "DU":
      FCO_DU_storage_position = 'FX'
      st_name = info.split(',')
      st_inv = []
      for name in st_name:
        cmd = "cyan -db cyclus.sqlite inv "
        st_inv += cyan(cmd + "-nucs=" + nucs_U + " " + name)
      st_inv = translate_info(st_inv, 2,timestep)
      st_inv_yearly = month2year(st_inv, 0, 0)/1000
      push_in_fco_excel(st_inv_yearly, r_sheet_storage, FCO_DU_storage_position, 6)
    elif st_id == "RU":
      FCO_RU_storage_position = 'FS'
      st_name = info.split(',')
      st_inv = []
      for name in st_name:
        cmd = "cyan -db cyclus.sqlite inv "
        st_inv += cyan(cmd + "-nucs=" + nucs_U + " " + name)
      st_inv = translate_info(st_inv, 2,timestep)
      st_inv_yearly = month2year(st_inv, 0, 0)/1000
      push_in_fco_excel(st_inv_yearly, r_sheet_storage, FCO_RU_storage_position, 6)
    elif st_id == "MA":
      FCO_RU_storage_position = 'FN'
      st_name = info.split(',')
      st_inv = []
      for name in st_name:
        cmd = "cyan -db cyclus.sqlite inv "
        st_inv += cyan(cmd + "-nucs=" + nucs_MA + " " + name)
      st_inv = translate_info(st_inv, 2,timestep)
      st_inv_yearly = month2year(st_inv, 0, 0)/1000
      push_in_fco_excel(st_inv_yearly, r_sheet_storage, FCO_RU_storage_position, 6)
    elif st_id == "PU":
      FCO_RU_storage_position = 'FI'
      st_name = info.split(',')
      st_inv = []
      for name in st_name:
        cmd = "cyan -db cyclus.sqlite inv "
        st_inv += cyan(cmd + "-nucs=" + nucs_Pu + " " + name)
      st_inv = translate_info(st_inv, 2,timestep)
      st_inv_yearly = month2year(st_inv, 0, 0)/1000
      push_in_fco_excel(st_inv_yearly, r_sheet_storage, FCO_RU_storage_position, 6)



def read_waste(hd, info):
  nucs_name = [ nucs_U, nucs_Pu, nucs_MA, nucs_FP]
  FCO_waste_position = ['HD', 'HI', 'HN', 'HS']
  r_sheet_waste = 'sheet4.xml'
  wt_inv = [ [], [], [], [] ]
  wt_name = info.split(',')
  # read Cooling information
  for name in wt_name:
    cmd = "cyan -db cyclus.sqlite inv "
    for i in range(4):
      wt_inv[i] += cyan(cmd + "-nucs=" + nucs_name[i] +" " + name)
  for i in range(4):
    wt_inv[i] = translate_info(wt_inv[i], 2,timestep)
    wt_inv_yearly = month2year(wt_inv[i], 0, 0)/1000
    push_in_fco_excel(wt_inv_yearly, r_sheet_waste, FCO_waste_position[i], 6)



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
  open_xslm("Output.xlsm")

  timestep = get_timestep()
  input_list = read_input(sys.argv[1])
  for line in input_list:
    if line != '\n':
      recover_info(line)

  save_xslm("test.xlsm")

#built_infotable()






main()
