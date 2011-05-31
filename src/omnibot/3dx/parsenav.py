#!/usr/bin/env python
import socket, sys

host = 'localhost'
startport = 2831
numports = 6

#fd = open('spacenav', 'r')
fd = sys.stdin

#Setup a comms socket and the destination ports
ports = []
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
for i in range(numports):
    ports.append( (host, startport+i) )

while True:
    l = fd.readline()
    l = l.split(',')
    num = []
    count = 0
    for pos in l:
        num.append( float(pos) )
        msg = 'SET ' + str( float(pos))
        s.sendto( msg, ports[count] )
        count += 1
    print num
