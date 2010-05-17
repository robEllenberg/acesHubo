import serial
import struct
import time

ser = serial.Serial(3, 1000000)  # open first serial port
print ser.portstr       # check which port was really used
ser.write("hello")      # write a string


snd = [0xFF,0xFF,0x1,0x2, 0x1, 0xFB]
data = []
for element in snd:
    data.append(struct.pack('B', element))
data = ''.join(data)

numRuns = 10000
runRange = range(numRuns)

t1 = time.time()
for i in runRange:
	ser.write(data)
	ser.read(6)
t2 = time.time()
ser.close()             # close port
print "total runtime was", t2-t1
print "average RTT was", (t2-t1)/numRuns