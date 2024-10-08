#!/bin/bash

# Archivo de entrada
archivo_entrada="lidar_filtrado.txt"
# Archivo de salida procesado
archivo_procesado="datos_procesados.txt"
# Archivo de imagen de salida
grafico_salida="grafico.png"
archivo_troncos="troncos_detectados.txt"

# Procesar el archivo
awk '{ if ($1 < 200 && $2 > 10) print 400, $2, $3; else print $1, $2, $3 }' "$archivo_entrada" > "$archivo_procesado"

# Generar el gráfico con gnuplot
#
gnuplot <<-EOFMarker
    set terminal pngcairo size 800,600
    set output "$grafico_salida"
    set xlabel "Marca de Tiempo (ms)"
    set ylabel "Distancia (cm)"
    set title "Distancia vs Marca de Tiempo"

    # Graficar distancia y troncos detectados
    plot "$archivo_procesado" using 3:1 with lines title "Distancia", \
         "troncos_detectados.txt" using 1:(150) with points pointtype 7 pointsize 1 linecolor rgb "red" title "Troncos Detectados"
EOFMarker
#         "troncos_detectados.txt" using 1:(400) with points pointtype 7 pointsize 1 linecolor rgb "red" title "Troncos Detectados"
#         "troncos_detectados.txt" using 1:(0) with points pointtype 7 pointsize 1 linecolor rgb "red" title "Troncos Detectados"
#
echo "Gráfico generado en $grafico_salida"

