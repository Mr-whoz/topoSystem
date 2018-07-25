# -*- coding: utf-8 -*-
#/usr/bin/python2.7
from recvfile import recvfile
from sendfile import sendfile
from tracert import tracert

import socket,time

if __name__ == "__main__":
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
    s.bind(('', 27015))
    s.listen(100)
    print 'opened server\n'
    # for i in range(1):
    while True:
        sock, addr = s.accept()
        print time.ctime(),':Connected by', addr
        
        recvfile(sock, 'host.txt')
        tracert('host.txt', 'result.txt')
        sendfile(sock, 'result.txt')
     
        sock.close()
        
