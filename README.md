# ryva


Instalacion de Debian oficial en Raspberry pi 3 model B
-------------------------------------------------------

```
wget https://people.debian.org/~gwolf/raspberrypi/20190628/20190628_raspberry-pi-3_buster_PREVIEW.img.xz

unxz 20190628_raspberry-pi-3_buster_PREVIEW.img.xz

# IMPORTANTE: en el siguiente comando, /dev/sde es el archivo de dispositivo de la memoria microsd conectada en la PC
cat 20190628_raspberry-pi-3_buster_PREVIEW.img | dd of=/dev/sde bs=4K status=progress

```

Encendemos y realizamos ssh a la raspberry. Si queremos conocer la IP podemos
hacer lo siguiente.

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



