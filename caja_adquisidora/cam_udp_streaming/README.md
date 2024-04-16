Embedded Video Data Acquisition and Streaming via UDP
=====================================================

Capturar en caja adquisidora fotos a disco:

 ./cliente_v4l2 --640 -w /home/rafa/fotos/ 
 ./cliente_v4l2 --848 -w /home/rafa/fotos/ 


Hay 3 programas en este proyecto.

cliente-udp-image-streaming 
---------------------------
Es el programa principal de captura.

   Captura frames (imágenes) a través de system calls ioctl al driver 
   de video for linux que controla la cámara YUYV o MJPEG. Cuando la cámara 
   indica que existe un frame disponible se desencola el buffer de lectura
 y se envía el frame por red via UDP.
   Este programa debería ser compilado y ejecutado en el sistema embebido
   que tendrá la cámara conectada.

   Está escrito unicamente en C y no utiliza ninguna biblioteca (utiliza la biblioteca lipjpeg, pero la hemos empotrado como parte de la aplicación).

```
# Compilar con:
cd cliente-udp-image-streaming/jpeg-9c
make
cd ..
make

# agregar (una unica vez) el usuario al grupo video. Ejemplo:
usermod -a -G video rafa

# Ejecutar:
./cliente_v4l2

# Ejecutar especificando servidor y puerto:
./cliente_v4l2 -d /dev/video4 -s 10.0.40.99 -p 8002

# otro ejemplo en banana pi
./cliente_v4l2 -d /dev/video0 -s 10.0.40.95 -p 8002
```

   

udp-image-streaming
-------------------
Es un servidor y visualizador de un streaming de video
    que venga por red via UDP.
    Este programa debería ser utilizado en PC, para visualizar la captura
    proveniente de los sistemas embebidos que ejecutan cliente-udp-streaming.

```
# hay que tener instalado las cabeceras de desarrollo de opencv
# ejemplo: apt-get install libopencv-dev

# Compilar con:
cd udp-image-streaming/
cmake .
make

# Ejecutar
./server 8000 2

# El comando anterior escucha en el puerto 8000 y escala el video a 2X
```

Obtener el streaming y enviarlo a disco en formato mp4
------------------------------------------------------

```
# El siguiente comando escucha en el puerto 8004 y envía los frames
# jpg a disco en formato de video MP4

nc -l -u 8004 |  ffmpeg -framerate ntsc -f image2pipe -c:v mjpeg -i - -pix_fmt yuv420p output.mp4

# En el sistema embebido habría que ejecutar cliente-udp-image-streaming,
# y especificar que envíe el video al sistema
# que escucha en el puerto 8004 a través del comando anterior
```


python-servidor-udp-image-streaming 
-----------------------------------
Este es un servidor sencillo en python
    que captura una imagen que venga por la red via UDP y la guarda a disco.

