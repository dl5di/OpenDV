import serial
import sys
import array
import thread
import time
import RPi.GPIO as GPIO

SERIAL_BAUD=230400
# Set Pin to toggle
RESET_PIN=7
PORT="/dev/ttyAMA0"
port = serial.Serial(PORT, baudrate=SERIAL_BAUD, timeout=1.0, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, xonxoff=False, rtscts=False, dsrdtr=False)
port.flushInput()
port.flushOutput()

def ambecommand(cmdarray):

	print ''.join('{:02x}'.format(x) for x in cmdarray)
	numout = port.write(cmdarray)
	print("Wrote: " + str(numout) + " bytes")
	print port.readline();

reset = bytearray.fromhex("61 00 01 00 33");
setDstar = bytearray.fromhex("61 00 0c 00 0a 01 30 07 63 40 00 00 00 00 00 00 48");
getProdId = bytearray.fromhex("61 00 01 00 30");
getVersion = bytearray.fromhex("61 00 01 00 31");
print 'Baudrate '.join('d'.format(port.baudrate))
print port.bytesize
print port.parity
print port.stopbits
print port.xonxoff
print port.rtscts
print port.dsrdtr
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(RESET_PIN, GPIO.OUT)
GPIO.setup(11, GPIO.IN)
GPIO.output(RESET_PIN, False)
time.sleep(0.02)
GPIO.output(RESET_PIN, True)
time.sleep(0.02)
rts = GPIO.input(11)
print rts
rcv = port.read(10)
sys.stdout.write(rcv)
print
print "Reset"
ambecommand(reset)
print "Product ID"
ambecommand(getProdId)
print "Version"
ambecommand(getVersion)
print "Set DSTAR Mode"
ambecommand(setDstar)

