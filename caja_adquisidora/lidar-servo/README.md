
Reporta:

angulo:distancia:delay

Si el delay es mayor a 13, y la distancia es menor a 3 metros, descartar
el valor (no hay obstaculos).


Para obtener desde la PC se puede :

Ejecutar en banana pi
stty -F /dev/ttyUSB0 speed 115200

Desde la PC:
cat /dev/ttyUSB0 | ./lidar_sdl





(c) 2022 - Rafael Ignacio Zurita <rafa@fi.uncoma.edu.ar>


# lidar-lite-v4-c
C code for controlling Garmin LIDAR lite v4
