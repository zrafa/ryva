# Distributed with a free-will license.
# Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
# ADXL345
# This code is designed to work with the ADXL345_I2CS I2C Mini Module available from ControlEverything.com.
# https://www.controleverything.com/content/Accelorometer?sku=ADXL345_I2CS#tabs-0-product_tabset-2
	
import smbus
import time
import py_qmc5883l
import math
import sys
sensor = py_qmc5883l.QMC5883L()
	
# Get I2C bus
bus = smbus.SMBus(2)
	
while True:
	# ADXL345 address, 0x53(83)
	# Select bandwidth rate register, 0x2C(44)
	#		0x0A(10)	Normal mode, Output data rate = 100 Hz
	bus.write_byte_data(0x53, 0x2C, 0x0A)
	# ADXL345 address, 0x53(83)
	# Select power control register, 0x2D(45)
	#		0x08(08)	Auto Sleep disable
	bus.write_byte_data(0x53, 0x2D, 0x08)
	# ADXL345 address, 0x53(83)
	# Select data format register, 0x31(49)
	#		0x08(08)	Self test disabled, 4-wire interface
	#					Full resolution, Range = +/-2g
	bus.write_byte_data(0x53, 0x31, 0x08)
	
	time.sleep(0.5)
	
	# ADXL345 address, 0x53(83)
	# Read data back from 0x32(50), 2 bytes
	# X-Axis LSB, X-Axis MSB
	data0 = bus.read_byte_data(0x53, 0x32)
	data1 = bus.read_byte_data(0x53, 0x33)
	
	# Convert the data to 10-bits
	xAccl = ((data1 & 0x03) * 256) + data0
	if xAccl > 511 :
		xAccl -= 1024
	
	# ADXL345 address, 0x53(83)
	# Read data back from 0x34(52), 2 bytes
	# Y-Axis LSB, Y-Axis MSB
	data0 = bus.read_byte_data(0x53, 0x34)
	data1 = bus.read_byte_data(0x53, 0x35)
	
	# Convert the data to 10-bits
	yAccl = ((data1 & 0x03) * 256) + data0
	if yAccl > 511 :
		yAccl -= 1024
	
	# ADXL345 address, 0x53(83)
	# Read data back from 0x36(54), 2 bytes
	# Z-Axis LSB, Z-Axis MSB
	data0 = bus.read_byte_data(0x53, 0x36)
	data1 = bus.read_byte_data(0x53, 0x37)
	
	# Convert the data to 10-bits
	zAccl = ((data1 & 0x03) * 256) + data0
	if zAccl > 511 :
		zAccl -= 1024
	
	# Output data to screen
	print "Acceleration in X-Axis : %d" %xAccl
	print "Acceleration in Y-Axis : %d" %yAccl
	print "Acceleration in Z-Axis : %d" %zAccl
	
	
	
	# ITG3200 address, 0x68(104)
	# Select Power management register 0x3E(62)
	#		0x01(01)	Power up, PLL with X-Gyro reference
	bus.write_byte_data(0x68, 0x3E, 0x01)
	# ITG3200 address, 0x68(104)
	# Select DLPF register, 0x16(22)
	#		0x18(24)	Gyro FSR of +/- 2000 dps
	bus.write_byte_data(0x68, 0x16, 0x18)
	
	time.sleep(0.5)
	
	# ITG3200 address, 0x68(104)
	# Read data back from 0x1D(29), 6 bytes
	# X-Axis MSB, X-Axis LSB, Y-Axis MSB, Y-Axis LSB, Z-Axis MSB, Z-Axis LSB
	data = bus.read_i2c_block_data(0x68, 0x1D, 6)
	
	# Convert the data
	xGyro = data[0] * 256 + data[1]
	if xGyro > 32767 :
		xGyro -= 65536
	
	yGyro = data[2] * 256 + data[3]
	if yGyro > 32767 :
		yGyro -= 65536
	
	zGyro = data[4] * 256 + data[5]
	if zGyro > 32767 :
		zGyro -= 65536
	
	# Output data to screen
	print "X-Axis of Rotation : %d" %xGyro
	print "Y-Axis of Rotation : %d" %yGyro
	print "Z-Axis of Rotation : %d" %zGyro


	x = sensor.get_magnet()
	# print x[0], x[1]
	declination = 4.75688
	heading = (math.atan2(x[1], x[0]) + declination )
	# Correct for when signs are reversed.
	if(heading < 0):
	    heading  = 2*math.pi;

	# Check for wrap due to addition of declination.
	if(heading > 2*math.pi):
	    heading -= 2*math.pi;

	heading = heading * (180 / math.pi)

	print heading
	sys.stdout.flush()
	time.sleep(0.2)
