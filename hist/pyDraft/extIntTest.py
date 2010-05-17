#!/usr/bin/env python

import extInt

def scaleUnitsTest():
    fromu = (10, 100)
    tou = (20, 222)
    
    for i in range(110):
        try:
            e = extInt.extInt(i, fromu)
            print i, '-->', e.onRange(tou)
        except:
            print "fail on ", i

def bigEndianTest():
    n = 1+2+4+8+16+32+64+128+256+512+1024
    e = extInt.extInt(n)
    for i in range(15):
        print e.bigEndian(i)
        
def __main__():
    scaleUnitsTest()
    bigEndianTest()

if (__name__=="__main__"):
    __main__()
