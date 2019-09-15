
Instalacion de Debian oficial en Raspberry pi 3 model B
=======================================================

```
wget https://people.debian.org/~gwolf/raspberrypi/20190628/20190628_raspberry-pi-3_buster_PREVIEW.img.xz

unxz 20190628_raspberry-pi-3_buster_PREVIEW.img.xz

# IMPORTANTE: en el siguiente comando, /dev/sde es el archivo de dispositivo de la memoria microsd conectada en la PC
cat 20190628_raspberry-pi-3_buster_PREVIEW.img | dd of=/dev/sde bs=4K status=progress

```

Los comandos anteriores instalarán una versión básica de Debian, sin 
desktop o aplicaciones gráficas, las cuales no son necesarias
en nuestro proyecto embebido de caja para captura de señales.

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


Adquisición del streaming de video desde cámaras
================================================

Se puede utilizar la aplicación en este repositorio para tal efecto.
Se encuentra bajo el directorio
``` cam_udp_streaming/ ```

Para comparativa se puede utilizar mplayer.

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


Compilación de ORB-SLAM2 en Raspbery pi con Debian
==================================================

Si se utiliza Debian buster entonces casi todas las dependencias
están en los repositorios de Debian. Así que no es complicada la cosa

```
# INSTALAMOS dependencias
apt-get install build-essential cmake
apt-get install git pkg-config libjpeg-dev libtiff5-dev  libpng-dev libtbb-dev
apt-get install libopencv-core3.2  libopencv-dev libeigen3-dev
apt-get install libblas-dev liblapack-dev libglew-dev

# Luego instalamos pangolin que no viene empaquetado para DEBIAN
git clone https://github.com/stevenlovegrove/Pangolin
cd Pangolin
mkdir build
cd build
cmake ..
make 
make install

# Crear un archivo de swap de 1G por las dudas. Y activarlo

# Luego, compilamos ORB-SLAM2 :

git clone https://github.com/raulmur/ORB_SLAM2.git ORB_SLAM2
cd ORB_SLAM2/
chmod a+x build.sh 

# Editar build.sh y cambiar make -j por simplemente make
# (esto evitará usar los 4 cores para make, que en raspberry produce
# sigalarms porque se queda sin memoria).

# Buscar todos los fuentes que tengan la funcion usleep
egrep -r usleep .

# Editar cada fuente y agregar:
#include <unisted.h>

# (eso evitará que de error el compilador por usleep. Igualmente usleep
# es obsoleto, así que la gente de orb-slam2 debería actualizar el código).

# Luego compilamos
./build.sh 

```
