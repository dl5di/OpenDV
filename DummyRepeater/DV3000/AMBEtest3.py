import serial
import sys
import getopt
import array
import thread
import time

def ambecommand(port,cmdarray):

	print ''.join('{:02x}'.format(x) for x in cmdarray)
	numout = port.write(cmdarray)
	print("Wrote: " + str(numout) + " bytes")
	print port.readline();

def main(argv):
	SERIAL_BAUD=230400
	serialport = "/dev/ttyAMA0"
	try:
		opts, args = getopt.getopt(argv,"hns:",["serial="])
	except getopt.GetoptError:
		print 'AMBEtest3.py -s <serial port>'
		print 'AMBEtest3.py -n -s <serial port> (for ThumbDV Model A'
		sys.exit(2)
	for opt, arg in opts:
		if opt == '-h':
			print 'AMBEtest3.py -s <serial port>'
			sys.exit()
		elif opt in ("-s", "--serial"):
			serialport = arg
		elif opt == "-n":
			SERIAL_BAUD=460800
	print 'Setting ...'
	port = serial.Serial(serialport, baudrate=SERIAL_BAUD, timeout=1.0, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, xonxoff=False, rtscts=False, dsrdtr=False)
	
	port.flushInput()
	port.flushOutput()
	print serialport
	
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
	time.sleep(0.02)
	rcv = port.read(10)
	sys.stdout.write(rcv)
	print
	print "Reset"
	ambecommand(port,reset)
	print "Product ID"
	ambecommand(port,getProdId)
	print "Version"
	ambecommand(port,getVersion)
	print "Set DSTAR Mode"
	ambecommand(port,setDstar)
	
if __name__ == "__main__":
	main(sys.argv[1:])
