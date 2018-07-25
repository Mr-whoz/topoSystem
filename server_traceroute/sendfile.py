# -*- coding: utf-8 -*-

import os

def sendfile(sock, file):
    f = open(file, 'rb')
    
    send_size = long(0)
    file_size = os.path.getsize(file)
    sock.sendall('%8u' % file_size)
    while send_size < long(file_size):
        read_data = f.read(1024)
        if not read_data: break
        
        curr_size = len(read_data)
        sock.sendall('%8u' % curr_size)
        sock.sendall(read_data)
        
        send_size += curr_size
        
    f.closed
    