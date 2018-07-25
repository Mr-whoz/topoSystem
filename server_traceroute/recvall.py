# -*- coding: utf-8 -*-

import socket

def recvall(sock, length):
    msg = ""
    while length:
        try:
            chunk = sock.recv(length)
        except socket.error:
            return ""
            
        if chunk == "": # connection closed
            return ""
            
        length -= len(chunk)
        msg += chunk
        
    return msg
    
