#! /usr/bin/env python
from __future__ import print_function, unicode_literals
import os
import sys
import tarfile
import subprocess
import shutil
import io



def update_describe():
    root_dir = os.path.split(__file__)[0]
    fname = os.path.join(root_dir, 'src', 'version.cc.in')
    cmd = 'touch {0}'.format(fname)
    subprocess.check_call(cmd.split(), shell=(os.name == 'nt'))



def main():
    output = "echo "
    prog = 'cyan'
    db = 'cyclus.sqlite'
    cmd = prog + ' -db ' + db + ' trans'
    print(cmd)

    output = subprocess.check_output(cmd.split())

    print(output)



main()