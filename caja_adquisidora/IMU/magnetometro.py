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
import threading






xAccl = 0
yAccl = 0
zAccl = 0

xGyro = 0
yGyro = 0
zGyro = 0

heading = 0.0



def adquirir_data():

#    global xAccl
#    global yAccl
#    global zAccl
#
#    global xGyro
#    global yGyro
#    global zGyro
##
#    global heading

    while True:


	x = sensor.get_magnet()
	declination = 4.75688
        x[0] = x[0] * 1.0 / 12000.0
        x[1] = x[1] * 1.0 / 12000.0
        x[0] = x[0] + 0.05850266
        x[1] = x[1] + 0.18496448
	heading = (math.atan2(x[1], x[0]) + declination )
	# Correct for when signs are reversed.
	if(heading < 0):
	    heading  = 2*math.pi;

	# Check for wrap due to addition of declination.
	if(heading > 2*math.pi):
	    heading -= 2*math.pi;

	heading2 = heading * (180 / math.pi)
        print int(round(time.time() * 1000.0)),heading2



# main(): 

sensor = py_qmc5883l.QMC5883L()

	
# Get I2C bus
bus = smbus.SMBus(2)
	

time.sleep(0.5)

time.sleep(0.5)


adquirir_data()


