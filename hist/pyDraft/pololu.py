#!/usr/bin/env python

import generic as g

class PololuProperty(g.Property):
    def __init__(self, module, id):
        g.Property.__init__(self, module)
        self.id = id
        
    def set(self, value):
        try:
            value = g.scaleOutgoing((self.lower, self.upper), self.byteLen, value, (0, 1023), MSBFirst=False)
        except:
            raise
        msg = WriteMessage(self.id, Constants.instructions['WRITE'], self.location, value)
        self.module.enqueue(msg)
        
    def get(self):
        try:
            pass

class PololuMessage():
    def __init__(self, id, instruction):
        self.id = id
        self.instruction = instruction
        
    def parameters(self):
        if (self.parameters):
            return self.parameters
        else:
            return []
    
    def checksum(self, packet):
        """Formula for computing checksum is ~(id + length + sum(parameter values) ).
        """
        val = 0;
        for p in packet:
            val += p.value
        val = c.c_ubyte(val)
        checksum = c.c_ubyte(~val.value)
        return checksum.value
    
    def bitstream(self):
        parameters = self.parameters()
        length = 2 + len(parameters)
        p = [c.c_ubyte(self.id), c.c_ubyte(length), c.c_ubyte(self.instruction)]
        p.extend(parameters)
        p.extend( [c.c_ubyte( self.checksum(p) )] )
        packet = [c.c_ubyte(0xFF), c.c_ubyte(0xFF)]
        packet.extend(p)
        return packet
    
class PololuModule():
    def __init__(self, bus):
        self.bus = bus
        self.queue = []

    def enqueue(self, msg):
        self.queue.append(msg)

    def condense(self, queue):
        pass
        
    def transmit(self):
		self.condense(self.queue)
		msg = self.queue.pop(0)
		bit = msg.bitstream()
		self.bus.transmit(bit)
