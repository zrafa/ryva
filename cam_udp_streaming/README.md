Video Data Aquisition from Cameras and Streaming via UDP
=======================================================

Hay 3 programas en este proyecto.

cliente-udp-image-streaming 
---------------------------
Es el programa principal de captura.
   Controla una camara YUYV o MJPEG via system calls al driver de video for
   linux que controla la cámara. Captura cada framee cuando el sensor indica
   que está disponible y lo envía por red via UDP.
   Este programa debería ser compilado y ejecutado en el sistema embebido
   que tendrá la cámara conectada.

python-servidor-udp-image-streaming 
-----------------------------------
Este es un servidor sencillo en python
    que captura una imagen que venga por la red via UDP y la guarda a disco.

udp-image-streaming
-------------------
Es un servidor y visualizador de un streaming de video
    que venga por red via UDP.
    Este programa debería ser utilizado en PC, para visualizar la captura
    proveniente de los sistemas embebidos que ejecutan cliente-udp-streaming.
