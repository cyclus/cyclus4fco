#! /usr/bin/env python
from __future__ import print_function, unicode_literals
import xlwings as xw
import os
import sys
import subprocess
import io
import numpy as np
import re
import openpyxl as xl

import cymetric as cym
global r_power_E
global r_deployed
global r_flow
global c_inv
global wr_inv
global mass_conversion

fco_sheet = ['Data-Front End', 'Data-Reactor', 'Data-Recycle', 'Data-Inventories', 'Data-Economics', 'Data-Wildcard', 'Graph_Data', 'Settings', 'Pres_Graphs', 'Graphs (C1-4)', 'Version Notes']

#Half-life > 1h
nucs_Pu_list = [ "238pu", "239pu", "240pu", "241pu", "242pu"]

nucs_U = "230U,231U,232U,233U,234U,235U,236U,237U,238U,240U"
nucs_Pu = "234Pu,235Pu,236Pu,237Pu,238Pu,239Pu,240Pu,241Pu,242Pu,243Pu,244Pu,245Pu,246Pu,247Pu"
nucs_Am = "239Am,240Am,241Am,242Am,243Am,244Am,245Am"
nucs_Cm = "238Cm,240Cm,241Cm,242Cm,243Cm,244Cm,245Cm,246Cm,247Cm,248Cm,250Cm"
nucs_Np = "234Np,235Np,236Np,237Np,238Np,239Np"
nucs_FP = "H3,GE74,AS75,GE76,SE77,SE78,SE79,SE80,BR81,SE82,KR82,KR83,KR84,KR85,RB85,KR86,SR86,RB87,SR88,SR89,Y89,SR90,ZR90,SR91,Y91,ZR91,ZR92,Y93,ZR93,ZR94,ZR95,NB95,MO95,ZR96,MO96,ZR97,MO97,MO98,MO99,TC99,TC99M,MO100,RU100,RU101,RU102,RU103,RH103,RU104,PD104,RU105,RH105,PD105,RU106,PD106,PD107,PD108,PD109,AG109,PD110,AG110,CD110,AG111,CD111,CD112,CD113,CD113,CD114,IN115,SN115,CD116,SN116,SN117,SN118,SN119,SN120,SB121,SN122,TE122,SN123,SB123,SN124,SB124,TE124,SN125,SB125,TE125,TE125,SN126,TE126,SB127,TE127,I127,TE128,XE128,TE129,I129,TE130,XE130,TE131,I131,XE131,XE131,TE132,I132,XE132,I133,XE133,XE133,CS133,XE134,CS134,BA134,I135,XE135,CS135,BA135,XE136,CS136,BA136,CS137,BA137,BA138,LA138,LA139,BA140,LA140,CE140,LA141,CE141,PR141,CE142,ND142,CE143,PR143,ND143,CE144,ND144,PR145,ND145,ND146,ND147,PM147,SM147,ND148,PM148,SM148,PM149,SM149,ND150,SM150,PM151,SM151,SM152,SM153,EU153,SM154,EU154,GD154,EU155,GD155,EU156,GD156,GD157,GD158,TB159,GD160,TB160,DY160,DY161,DY162,DY163,DY164,HO165,ER166"
nucs_MA = nucs_Am + "," + nucs_Cm + "," + nucs_Np



def push_in_excel(matrix, sheet, column, start_row):
    for i in range(len(matrix)):
        cmd = "sed 's/\(r=\"" + column + str(start_row + i)
        cmd += "\" .......<v>\)[^<]*/\\1"
        cmd += str(matrix[i]) + "/' _tmp/xl/worksheets/" + \
            sheet + " > _tmp/xl/worksheets/sheet_tmp.xml"
        os.system(cmd)
        os.system("mv _tmp/xl/worksheets/sheet_tmp.xml _tmp/xl/worksheets/" + sheet)
	
def save_xslm(filename):
    cmd = 'cd _tmp; zip -r ../' + filename + ' *'
    os.system(cmd + ' >/dev/null')
    cmd = 'rm -rf _tmp'
    os.system(cmd)

def open_xslm(filename):
    cmd = 'unzip ' + filename + ' -d _tmp/'
    output = subprocess.check_output(cmd.split())

def write_exel(vals, xl_file, column, sheet, start_row):
    open_xslm(xl_file)
    push_in_excel(vals, sheet, column, start_row)
    save_xslm(xs_file)



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
  return output



def process(evaler, file):
    
  for line in file:
    if line[0] = '#':
        continue
    else if line[0] != '%':
        param val = line[1:].split(":",1)
        if param == 'startrow':
            startrow = int(val)
        else if param = 'power':
            time_conversion = foat(val)
        else if param = 'mass':
            mass_conversion = foat(val)
        else if param = 'power':
            power_conversion = foat(val)
        else:
            exec_line(evaler, line)

def get_vals(evaler, param):
    key = param[2]

# clean empty parameters
    for i in range(len(param))[3:]:
        if param[i] == ['']: param[i][:] = []
    fac1 = param[3]
    fac2 = param[4]
    nucs = param[5]

    if key == "inv":
        val = cytim.inventories(ev, facilities=fac1,nucs=nucs)
        val = val['Mass'].copy() * mass_conversion
    else if key == "trans":
        val = cytim.transactions(ev, senders=fac1, receivers=fac2, nucs=nucs)
        val = val['Mass'].copy() * mass_conversion
    else if key == "power":
        val = cytim.get_power(evaler, facilities=fac1)
        val = power['Value'].copy() * power_conversion 
    else if kay = "time":
        val = evaler.eval('TimeList').copy() * time_conversion

    return val.tolist()
  

def exec_line(evaler, line):
    param = line.split(':',1)
    vals = get_val(evaler, param)
    write_exel(vals, param[0], param[1], start_row)


def month2year(val, mode=0, division=12):
    out_val = []
    val = 0
    for i  in (range(len(vals)):
        if mode == 0:
            val = vals[i]
        else:
            val += vals[i]
        if index % division == 0:
            if mode == 2:
                val *= 1./double(division)
            out_val.append(val)
            val = 0
    return out_val


def get_timestep():
    timestep = get_val(ev, ['time'])
    timestep = month2year(timestep, 0)/12. 
    return timestep



def main():
  global timestep
  if len(sys.argv) != 2 :
    print("missing argument !!")
    quit()

  input_list = read_input(sys.argv[1])
  process(input_list)

#built_infotable()






main()
