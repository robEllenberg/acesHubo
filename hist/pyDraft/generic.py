#!/usr/bin/env python

import ctypes as c
		
class Property():
	nextID = 0
	def __init__(self, module):
		self.propID = Property.nextID
		Property.nextID += 1
		self.module = module
		#router = module.router
		#router.register(self.propID, module)
		#self.router = router

def scaleOutgoing(fromUnits, byteLen, value, toUnits=None, MSBFirst=True):
    if(not toUnits):
        byte_upper = pow(2, 8*byteLen)-1
        toUnits = (0, byte_upper)
    val = scale_units(fromUnits, toUnits, value, True)
    outgoing = []
    for i in range(byteLen):
        mask = (255 << 8*i)
        #print mask
        v = val & mask
        #print v
        v = (v >> 8*i)
        outgoing.append(v)
    if(MSBFirst):
        outgoing.reverse()
    #print outgoing
    return outgoing
	
def scale_units(fromUnits, toUnits, value, doround=False):
    """Linearly scales a number on the range [fromUnitLow, fromUnitHigh] to
    the range [toUnitLow, toUnitHigh].
    Unit - the value to be scaled
    doround - wheather or not to round the resultant value. True for yes,
    False for no
    This function should always be used in conjunction w/checkrange()
    """
    checkrange(value, fromUnits[0], fromUnits[1])
    torange = float(toUnits[1]) - float(toUnits[0])
    fromrange = float(fromUnits[1]) - float(fromUnits[0])
    ratio = torange/fromrange
    
    unit = float(value)
    percentage_interval = (unit-float(fromUnits[0]))/ fromrange
    result = torange*percentage_interval + float(toUnits[0])
    
    if(doround):
        return int(round(result))
    else:
        return result
        
def checkrange(value, low, high):
    """A simple function for ensuring that a certain value is within a
    specified range.
    command - string w/identifier for the command
    value - the value to check
    high - highest value allowed 
    low - lowest value allowed
    """
    if( (value < low) or (value > high)):
        raise RangeError(value, low, high)

class RangeError(Exception):
	def __init__(self, value, low, high):
		self.value = value
		self.low = low
		self.high = high
	def __str__(self):
		range = ["(",str(self.low),", ",str(self.high),")"]
		range = ''.join(range)
		return ''.join(["The value ", str(self.value), " is outside the range: ", range])
		
"""        
class Router():
	def __init__(self):
		self.routeTable = {}
		pass
	def register(id, module):
		if self.routeTable[id]:
			raise
		else:
			self.routeTable[id] = module
	def enqueue(self, sourceID, msg):
		self.routeTable[sourceID].enqueue(msg)
"""		
