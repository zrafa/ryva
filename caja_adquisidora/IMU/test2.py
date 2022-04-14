#!/usr/bin/python
import time
import py_qmc5883l
import math
import sys
sensor = py_qmc5883l.QMC5883L()
while True:
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
