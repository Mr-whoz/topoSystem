# -*- coding: utf-8 -*-

from recvall import recvall

import os

def recvfile(sock, file):
    if os.path.exists(file):
        os.remove(file)
        
    f = open(file, 'wb')
    file_size = recvall(sock, 8)
    recv_size = long(0)
    while recv_size < long(file_size):
        curr_size = recvall(sock, 8)
        if curr_size == "":
            break
            
        curr_size = long(curr_size)
        
        data = recvall(sock, curr_size)
        if data == "":
            break
            
        f.write(data)
        
        recv_size += curr_size
        
    f.closed
    
