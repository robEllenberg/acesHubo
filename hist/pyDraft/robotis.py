#!/usr/bin/env python

import generic as g
import ctypes as c

class Constants(object):
    instructions = {'PING':0x1 ,'READ':0x2, 'WRITE':0x3, 'RWRITE':0x4, 'ACTION':0x5, 'RESET':0x6, 'SWRITE':0x83}
    
    MODEL_NUMBER = 0x0
    VERSION_OF_FIRMWARE = 0x2
    ID = 0x3
    BAUD_RATE = 0x4
    RETURN_DELAY_TIME = 0x5
    CW_ANGLE_LIMIT = 0x6
    CCW_ANGLE_LIMIT = 0x8
    HIGHEST_LIMIT_TEMPERATURE = 0xB
    LOWEST_LIMIT_VOLTAGE = 0xC
    HIGHEST_LIMIT_VOLTAGE = 0xD
    MAX_TORQUE = 0xE
    STATUS_RETURN_LEVEL = 0x10
    ALARM_LED = 0x11
    ALARM_SHUTDOWN = 0x12

    TORQUE_ENABLE = 0x18
    LED = 0x19
    CW_COMPLIANCE_MARGIN = 0x1A
    CCW_COMPLIANCE_MARGIN = 0x1B
    CW_COMPLIANCE_SCOPE = 0x1C
    CCW_COMPLIANCE_SCOPE = 0x1D
    GOAL_POSITION = 0x1E
    MOVING_SPEED = 0x20
    TORQUE_LIMIT = 0x22
    PRESENT_POSITION = 0x24
    PRESENT_SPEED = 0x26
    PRESENT_LOAD = 0x28
    PRESENT_VOLTAGE = 0x2A
    PRESENT_TEMPERATURE = 0x2B
    REGISTERED_INSTRUCTION = 0x2C
    MOVING = 0x2E
    LOCK = 0x2F
    PUNCH = 0x30

class RobotisProperty(g.Property):
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
            
"""
class RobotisMotor(object):
    def __init__(self, id):
        self.props = {}
        for name in ctrlTbl:
            self.props[name] = RobotisProperty( module, id, ctrlTbl[name] )
        
        self.id = id
        self.pos = position(id)
        self.led = led(id)

"""        
class RobotisPosition(RobotisProperty):
    def __init__(self, module, id):
        RobotisProperty.__init__(self, module, id)
        self.location = Constants.GOAL_POSITION
        self.lower = 0
        self.upper = 300
        self.byteLen = 2

class RobotisMessage():
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
        
class WriteMessage(RobotisMessage):
    def __init__(self, id, instruction, address, data):
        RobotisMessage.__init__(self, id, instruction)
        self.address = address
        self.data = data
        
    def parameters(self):
        data = []
        for d in self.data:
            data.append(c.c_ubyte(d))
        n = [c.c_ubyte(self.address)]
        n.extend(data)
        return n

class ReadMessage(RobotisMessage):
    def __init__(self, id, instruction, address, length):
        RobotisMessage.__init__(self, id, instruction)
        self.address = address
        self.length = length
        
    def parameters(self):
        return [c.c_ubyte(self.address), c.c_ubyte(self.length)]

class ResetMessage(RobotisMessage):
    def __init__(self, id):
        RobotisMessage.__init__(self, id, Constants.instructions['RESET'])
        
class RobotisModule():
    def __init__(self, bus):
        self.bus = bus
        self.queue = []

    def enqueue(self, msg):
        self.queue.append(msg)

    def condense(self, queue):
        pass
    """
    def condense(queue):
		q2 = []
		#we do this backwards so the indecies don't change as we progress through the list
		for item in reverse(range(len(queue))):
			if(queue[item].instruction == Message.instructions['WRITE']):
				if():
	"""
			
    def transmit(self):
		self.condense(self.queue)
		msg = self.queue.pop(0)
		bit = msg.bitstream()
		self.bus.transmit(bit)
        
class RobotisBus():
    def __init__(self):
        pass
    def transmit(self, msg):
        print msg
