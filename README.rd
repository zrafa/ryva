# ryva


Instalacion de Debian oficial en Raspberry pi 3 model B
-------------------------------------------------------

```
wget https://people.debian.org/~gwolf/raspberrypi/20190628/20190628_raspberry-pi-3_buster_PREVIEW.img.xz

unxz 20190628_raspberry-pi-3_buster_PREVIEW.img.xz

cat 20190628_raspberry-pi-3_buster_PREVIEW.img | dd of=/dev/sde bs=4K status=progress
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



