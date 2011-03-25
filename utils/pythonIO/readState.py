#!/usr/bin/env python

import socket
import sys
import time

UDP_IP = "127.0.0.1"
UDP_PORT = int(sys.argv[1])
BUFFER_SIZE = 1024
MESSAGE = "GET"

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.connect((UDP_IP, UDP_PORT))
#s2.bind((UDP_IP, UDP_PORT))

while range(100):
    time.sleep(.1)
    data = False
    s.sendall(MESSAGE)
    while not data:
        data = s.recv(BUFFER_SIZE)
        print data 
        sys.stdout.flush()
