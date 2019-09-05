
Instalacion de Debian oficial en Raspberry pi 3 model B
-------------------------------------------------------

```
wget https://people.debian.org/~gwolf/raspberrypi/20190628/20190628_raspberry-pi-3_buster_PREVIEW.img.xz

unxz 20190628_raspberry-pi-3_buster_PREVIEW.img.xz

# IMPORTANTE: en el siguiente comando, /dev/sde es el archivo de dispositivo de la memoria microsd conectada en la PC
cat 20190628_raspberry-pi-3_buster_PREVIEW.img | dd of=/dev/sde bs=4K status=progress

```

Encendemos y realizamos ssh a la raspberry. Si queremos conocer la IP podemos
hacer lo siguiente:

```
# ANTES de encender la raspberry
for i in `seq 1 254` ; do ping -c 1 10.0.40.$i && echo $i ANDA & echo $i ; done | grep ANDA | sort -n
# el comando anterior nos indica todas las IP de la red

# ENCENDEMOS la raspberry, y luego
for i in `seq 1 254` ; do ping -c 1 10.0.40.$i && echo $i ANDA & echo $i ; done | grep ANDA | sort -n
# verificamos ahora cual es la nueva IP que aparecio' en la red

ssh root@IP_raspberry
# clave de root "raspberry"

```

Para conocer detalles de la camara
----------------------------------

```
sudo apt install v4l-utils
v4l2-ctl -D
v4l2-ctl --list-formats-ext
v4l2-ctl -V
v4l2-ctl -stream-mmap=3 --stream-count=1000 --stream-to=/dev/null
```


Capturador de frames desde cámaras
==================================

Mplayer
-------

Mplayer es un reproductor famoso de todas las distribuciones Linux.
Puede reproducir desde dispositivos v4l2, y tiene buena performance de captura
para comparar.

```
apt-get install mplayer

mplayer  tv:// -tv driver=v4l2:device=/dev/video0:width=640:height=480
time mplayer  tv:// -tv driver=v4l2:device=/dev/video0:width=640:height=480
# Con el último comando podemos tomar el tiempo exacto de ejecucion
# Ademas, mplayer reporta la cantidad de frames obtenidos y procesados
```

Ejemplo:

```
En PC, camara facecam 1320 (soporta 30fps 640x480)
mplayer:
time mplayer  tv:// -tv driver=v4l2:device=/dev/video0:width=640:height
Tiempo de ejecución:
27.579 segundos
Cantidad de frames procesados:
802 frames , 14 descartados
802/27.5= 29.16363636363636363636 fps

```


En raspberry, camara facecam 1320, 640x480
No funciona, tira todo el tiempo select timeout (igual que con nuestro
programa).


En raspberry, camara facecam 1320, 320x240:
time mplayer -tv driver=v4l2:device=/dev/video0:width=320:height=240 -vo null tv://

v4l2: 213 frames successfully processed, 220 frames dropped.

Exiting... (Quit)

real	0m16.349s
user	0m1.823s
sys	0m0.156s

Aproximadamente 12 frames por segundo, igual que con nuestro programa.

