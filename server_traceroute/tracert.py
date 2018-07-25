# -*- coding: utf-8 -*-

import platform
import os

def tracert(infile, outfile):
    if os.path.exists(outfile):
        os.remove(outfile)
        
    system = platform.system().lower()
    f=open(infile, 'r')
    for line in f:
        line = line.strip('\n')
        if system == 'windows':
            os.system('tracert ' + line + ' >> ' + outfile)
        elif system == 'linux':
            os.system('traceroute ' + line + '-n' + ' >> ' + outfile)
        else:
            raise('Not is windows or linux ?')
    f.closed
    