#!/usr/bin/env python

class extInt(int):
    def __new__(self, val, range=(None, None)):
        #range of (None,None) represents an unbounded interval (-inf, +inf)
        if (range[0] and (val < range[0])):
            raise Exception()
        elif (range[1] and (val > range[1])):
            raise Exception()
        else:
            a = int.__new__(extInt, val)
            a.range = range
            return a

    def __init__(self, val, range=(None, None)):
        pass

    def onRange(self, newRange):
        if (self.range == (None,None) ):
            if (self.real < range[0]):
                raise
            elif (self.real > range[1]):
                raise
            else:
                self.range = newRange
                return self.real
        elif (self.range[0] == None):
            pass
        elif (self.range[1] == None):
            pass
        else:
            fromUnits = self.range
            toUnits = newRange
            torange = float(toUnits[1]) - float(toUnits[0])
            fromrange = float(fromUnits[1]) - float(fromUnits[0])
            ratio = torange/fromrange
            unit = float(self.real)
            percentage_interval = (unit-float(fromUnits[0]))/ fromrange
            result = torange*percentage_interval + float(toUnits[0])
            
            return int(round(result))

    def bigEndian(self, numBytes):
        n = pow(2, numBytes)-1
        res = self.real & n
        return res
        
    def littleEndian(self, numBytes):
        raise Exception
