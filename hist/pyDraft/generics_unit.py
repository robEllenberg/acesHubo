#!/usr/bin/env python

import robotis 
import generic as g

def test1():
    #r1 = r5.Router()
    b1 = robotis.RobotisBus()
    m1 = robotis.RobotisModule(b1)
    p1 = robotis.RobotisPosition(m1, 1)
    print p1.propID
    p1.set(300)
    m1.transmit()

def scaleTest():
    fromu = (10, 100)
    tou = (20, 222)
    
    for i in range(110):
        try:
            print i, '-->', g.scale_units( fromu, tou, i, False )
        except g.RangeError as e:
            print str(e)
            
    for i in range (110):
        try:
            print i, '-->', g.scale_units( fromu, tou, i, True )
        except:
            print str(e)
        
def __main__():
    test1()
    #scaleTest()

if __name__=="__main__":
    __main__()
