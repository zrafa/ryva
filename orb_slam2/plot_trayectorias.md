

Sin querer generar una guerra de religiones entre matlab y gnuplot
aporto un pequeño script para graficar las trayectorias desde Linux
con gnuplot (hoy Dario me explicó que cada línea de la trayectoria de
orb_slam2 es una matrix de 3x4 y  que la cuarta columna es la que se
utiliza).

Al margen: Mientras que matlab es para muchas cosas, gnuplot es una
herramienta en linux estándar para graficar resultados (sobre todo
para incorporarlo en presentaciones o papers).

El paquete tiene pocas dependencias (está en todos los linux) y se
utiliza unicamente para eso (para graficar y nada más).

Adjunto una gráfica confeccionada con gnuplot (no sé si tomé las
columnas correctas, creo que sí por la forma).
El script que genera la gráfica adjunta lo subí al repositorio git
compartido (por si lo quieren probar en algún momento para graficar:
https://github.com/zrafa/ryva/blob/master/orb_slam2/plot_trayectorias.gp

Para generar las gráficas desde linux simplemente se edita un script
como el anterior, y desde la terminal (teniendo instalado gnuplot):

```
gnuplot plot_trayectorias.gp
```


