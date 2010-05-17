#!/usr/bin/env python
import ctypes as c
import sys
import struct

fileName = sys.argv[1]
print "filename is", fileName

fIn = open(fileName, 'r')
fOut = open(fileName+'-binned', 'wb')

fInData = fIn.readlines()
print fInData
fIn.close()
fIn = fInData[0].split(' ')
print fIn

data = []
for element in fIn:
    data.append( struct.pack('B', int(element, 16)) )
data = ''.join(data)
fOut.write(data)
fOut.close()
